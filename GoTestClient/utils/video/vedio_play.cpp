#include "vedio_play.h"

#include <QCameraInfo>
#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QDateTime>
#include <QThread>
#include <QTimer>

extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavdevice/avdevice.h"    // 调用输入设备需要的头文件
}

#define ERROR_LEN 1024  // 异常信息数组长度
#define PRINT_LOG 1

VedioPlay::VedioPlay(QObject *parent)
{

}
/**
 * @brief
 * @return
 */
AVFrame* VedioPlay::read()
{
    QMutexLocker lock(&m_mutex);
    if(!m_timer->isActive()){
        m_timer->start(10);
    }
    // 如果没有打开则返回
    if(!m_formatContext)
    {
        return nullptr;
    }
    //    while (1) {

    //        qDebug()<<QThread::currentThreadId();
    // 读取下一帧数据
    int readRet = av_read_frame(m_formatContext, m_packet);
    if(readRet < 0)
    {
        avcodec_send_packet(m_codecContext, m_packet); // 读取完成后向解码器中传如空AVPacket，否则无法读取出最后几帧
    }
    else
    {
        if(m_packet->stream_index == m_videoIndex)     // 如果是图像数据则进行解码
        {
            // 计算当前帧时间（毫秒）
#if 1       // 方法一：适用于所有场景，但是存在一定误差
            m_packet->pts = qRound64(m_packet->pts * (1000 * rationalToDouble(&m_formatContext->streams[m_videoIndex]->time_base)));
            m_packet->dts = qRound64(m_packet->dts * (1000 * rationalToDouble(&m_formatContext->streams[m_videoIndex]->time_base)));
#else       // 方法二：适用于播放本地视频文件，计算每一帧时间较准，但是由于网络视频流无法获取总帧数，所以无法适用
            m_obtainFrames++;
            m_packet->pts = qRound64(m_obtainFrames * (qreal(m_totalTime) / m_totalFrames));
#endif
            // 将读取到的原始数据包传入解码器
            int ret = avcodec_send_packet(m_codecContext, m_packet);
            if(ret < 0)
            {
                showError(ret);
            }
        }
    }
    av_packet_unref(m_packet);  // 释放数据包，引用计数-1，为0时释放空间

    av_frame_unref(m_frame);
    int ret = avcodec_receive_frame(m_codecContext, m_frame);
    if(ret < 0)
    {
        av_frame_unref(m_frame);
        if(readRet < 0)
        {
            m_end = true;     // 当无法读取到AVPacket并且解码器中也没有数据时表示读取完成
        }
        return nullptr;
    }
    auto pkt = m_encode->encode(m_frame);
    if(!pkt){
        return nullptr;
    }
    auto mes = QDateTime::currentMSecsSinceEpoch();

    QByteArray encodeData;
    encodeData.append((char*)&mes,8);
    encodeData.append((char*)pkt->data,pkt->size);
//    qDebug()<<"VedioPlay encode mes = "<<mes;
    if(m_startCommunication)
        m_lhtMqttPusher->LhtMqttPush(encodeData);
    //    QThread::msleep(10);
    //        auto frame = m_decode->decode(pkt);
    //转换为RGB888
    sws_scale(m_imgConvertCtx, (const uint8_t * const *)m_frame->data, m_frame->linesize,
              0, m_codecContext->height, m_frameRGB->data, m_frameRGB->linesize);


    //        AVFrame *pFrameRGBCopy = av_frame_alloc();
    //        av_image_copy(pFrameRGBCopy->data, pFrameRGBCopy->linesize, (const uint8_t **)m_frameRGB->data, m_frameRGB->linesize,
    //                      AV_PIX_FMT_RGB24, m_codecContext->width, m_codecContext->height);
    //本地显示一下
    QImage image(m_frameRGB->data[0], m_codecContext->width, m_codecContext->height, QImage::Format_RGB888);
    signImageReadReady(image);
    //    img = image.copy();
    m_pts = m_frame->pts;
    //    }


    return m_frame;
}

void VedioPlay::startCommunication()
{
    m_startCommunication = 1;
}

/**
 * @brief 初始化ffmpeg库（整个程序中只需加载一次）
 *        旧版本的ffmpeg需要注册各种文件格式、解复用器、对网络库进行全局初始化。
 *        在新版本的ffmpeg中纷纷弃用了，不需要注册了
 */
void VedioPlay::initFFmpeg()
{
    static bool isFirst = true;
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if(isFirst)
    {
        //        av_register_all();         // 已经从源码中删除
        /**
         * 初始化网络库,用于打开网络流媒体，此函数仅用于解决旧GnuTLS或OpenSSL库的线程安全问题。
         * 一旦删除对旧GnuTLS和OpenSSL库的支持，此函数将被弃用，并且此函数不再有任何用途。
         */
        //        avformat_network_init();
        // 初始化libavdevice并注册所有输入和输出设备。
        avdevice_register_all();
        isFirst = false;
    }
}

/**
 * @brief      打开媒体文件，或者流媒体，例如rtmp、strp、http
 * @param url  视频地址
 * @return     true：成功  false：失败
 */
bool VedioPlay::open(const QString &url,QString ip , int port,int pushId,int recvId)
{
    QMutexLocker lock(&m_mutex);
    m_ip = ip;
    m_port = port;
    m_pushId = pushId;
    m_recvId = recvId;
    initFFmpeg();      // 5.1.2版本不需要调用了
    m_encode = new X264Encode(LhtEncode);
    m_encode->initEncoder();
    m_decode = new X264Decode();
    m_decode->initDecoder();
    m_error = new char[ERROR_LEN];


    /**
     * dshow：  Windows 媒体输入设备。目前仅支持音频和视频设备。
     * gdigrab：基于 Win32 GDI 的屏幕捕获设备
     * video4linux2：Linux输入视频设备
     */
#if defined(Q_OS_WIN)
    m_inputFormat = av_find_input_format("dshow");            // Windows下如果没有则不能打开摄像头
#elif defined(Q_OS_LINUX)
    m_inputFormat = av_find_input_format("video4linux2");       // Linux也可以不需要就可以打开摄像头
#elif defined(Q_OS_MAC)
    m_inputFormat = av_find_input_format("avfoundation");
#endif

    if(!m_inputFormat)
    {
        qWarning() << "查询AVInputFormat失败！";
    }
    if(url.isNull()) return false;


    AVDictionary* dict = nullptr;
    /**
     * Windows：可使用【ffmpeg -list_options true -f dshow -i video="Lenovo EasyCamera"】命令查看摄像头支持的编码器、帧率、分辨率等信息
     * Linux：可使用【ffmpeg -list_formats all -i /dev/video0】或【ffplay -f video4linux2 -list_formats all /dev/video0】命令查看摄像头支持的支持的像素格式、编解码器和帧大小
     */
    // 设置解码器（Linux下打开本地摄像头默认为rawvideo解码器，输入图像为YUYV420，不方便显示，有两种解决办法，1：使用sws_scale把YUYV422转为YUVJ422P；2：指定mjpeg解码器输出YUVJ422P图像）
    //    av_dict_set(&dict, "input_format", "mjpeg", 0);
    av_dict_set(&dict, "framerate", "30", 0);             // 设置帧率
    //    av_dict_set(&dict, "pixel_format", "yuvj422p", 0);   // 设置像素格式
    av_dict_set(&dict, "video_size", "640x480", 0);       // 设置视频分辨率（如果该分辨率摄像头不支持则会报错）
    auto ifmt=av_find_input_format("dshow");

    // 打开输入流并返回解封装上下文
    char strUrl[100]{0};
    memcpy(strUrl,url.toLocal8Bit().data(),url.toLocal8Bit().length());
    int ret = avformat_open_input(&m_formatContext,          // 返回解封装上下文
                                  strUrl,  // 打开视频地址
                                  ifmt,             // 如果非null，此参数强制使用特定的输入格式。自动选择解封装器（文件格式）
                                  &dict);                    // 参数设置

    // 释放参数字典
    if(dict)
    {
        av_dict_free(&dict);
    }
    // 打开视频失败
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    // 读取媒体文件的数据包以获取流信息。
    ret = avformat_find_stream_info(m_formatContext, nullptr);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }
    m_totalTime = m_formatContext->duration / (AV_TIME_BASE / 1000); // 计算视频总时长（毫秒）
#if PRINT_LOG
    qDebug() << QString("视频总时长：%1 ms，[%2]").arg(m_totalTime).arg(QTime::fromMSecsSinceStartOfDay(int(m_totalTime)).toString("HH:mm:ss zzz"));
#endif

    // 通过AVMediaType枚举查询视频流ID（也可以通过遍历查找），最后一个参数无用
    m_videoIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if(m_videoIndex < 0)
    {
        showError(m_videoIndex);
        free();
        return false;
    }

    AVStream* videoStream = m_formatContext->streams[m_videoIndex];  // 通过查询到的索引获取视频流

    videoStream->codecpar->width = VIDEO_WIDTH;
    videoStream->codecpar->height = VIDEO_HEIGHT;
    // 获取视频图像分辨率（AVStream中的AVCodecContext在新版本中弃用，改为使用AVCodecParameters）
    m_size.setWidth(VIDEO_WIDTH);
    m_size.setHeight(VIDEO_HEIGHT);
    m_frameRate = rationalToDouble(&videoStream->avg_frame_rate);  // 视频帧率

    // 通过解码器ID获取视频解码器（新版本返回值必须使用const）
    const AVCodec* codec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    m_totalFrames = videoStream->nb_frames;

#if PRINT_LOG
    qDebug() << QString("分辨率：[w:%1,h:%2] 帧率：%3  总帧数：%4  解码器：%5")
                .arg(m_size.width()).arg(m_size.height()).arg(m_frameRate).arg(m_totalFrames).arg(codec->name);
#endif

    // 分配AVCodecContext并将其字段设置为默认值。
    m_codecContext = avcodec_alloc_context3(codec);
    if(!m_codecContext)
    {
#if PRINT_LOG
        qWarning() << "创建视频解码器上下文失败！";
#endif
        free();
        return false;
    }

    // 使用视频流的codecpar为解码器上下文赋值
    ret = avcodec_parameters_to_context(m_codecContext, videoStream->codecpar);
    m_codecContext->width = VIDEO_WIDTH;
    m_codecContext->height = VIDEO_HEIGHT;
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    m_codecContext->flags2 |= AV_CODEC_FLAG2_FAST;    // 允许不符合规范的加速技巧。
    m_codecContext->thread_count = 8;                 // 使用8线程解码

    // 初始化解码器上下文，如果之前avcodec_alloc_context3传入了解码器，这里设置NULL就可以
    ret = avcodec_open2(m_codecContext, nullptr, nullptr);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    // 分配AVPacket并将其字段设置为默认值。
    m_packet = av_packet_alloc();
    if(!m_packet)
    {
#if PRINT_LOG
        qWarning() << "av_packet_alloc() Error！";
#endif
        free();
        return false;
    }
    // 分配AVFrame并将其字段设置为默认值。
    m_frame = av_frame_alloc();
    if(!m_frame)
    {
#if PRINT_LOG
        qWarning() << "av_frame_alloc() Error！";
#endif
        free();
        return false;
    }

    // 分配图像空间
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_size.width(), m_size.height(), 4);
    /**
     * 【注意：】这里可以多分配一些，否则如果只是安装size分配，大部分视频图像数据拷贝没有问题，
     *         但是少部分视频图像在使用sws_scale()拷贝时会超出数组长度，在使用使用msvc debug模式时delete[] m_buffer会报错（HEAP CORRUPTION DETECTED: after Normal block(#32215) at 0x000001AC442830370.CRT delected that the application wrote to memory after end of heap buffer）
     *         特别是这个视频流http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4
     */
    m_buffer = new uchar[size + 1000];    // 这里多分配1000个字节就基本不会出现拷贝超出的情况了，反正不缺这点内存
    //    m_image = new QImage(m_buffer, m_size.width(), m_size.height(), QImage::Format_RGBA8888);  // 这种方式分配内存大部分情况下也可以，但是因为存在拷贝超出数组的情况，delete时也会报错
    m_end = false;

    int numBytes;
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, m_codecContext->width, m_codecContext->height, 1);
    m_rgbbuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    m_rgbRemoteBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    m_frameRGB = av_frame_alloc();
    av_image_fill_arrays(m_frameRGB->data, m_frameRGB->linesize, m_rgbbuffer, AV_PIX_FMT_RGB24, m_codecContext->width, m_codecContext->height, 1);
    m_frameRemoteRGB = av_frame_alloc();
    av_image_fill_arrays(m_frameRemoteRGB->data, m_frameRemoteRGB->linesize, m_rgbRemoteBuffer, AV_PIX_FMT_RGB24, m_codecContext->width, m_codecContext->height, 1);
    m_imgConvertCtx = sws_getContext(m_codecContext->width, m_codecContext->height, m_codecContext->pix_fmt,
                                     m_codecContext->width, m_codecContext->height, AV_PIX_FMT_RGB24,
                                     SWS_BICUBIC, NULL, NULL, NULL);
    m_imgConvertRemoteCtx = sws_getContext(m_codecContext->width, m_codecContext->height, m_codecContext->pix_fmt,
                                     m_codecContext->width, m_codecContext->height, AV_PIX_FMT_RGB24,
                                     SWS_BICUBIC, NULL, NULL, NULL);

    m_lhtMqttPusher = new LhtMqttPusher;

    m_lhtMqttReceiver = new LhtMqttReceiver;

    m_lhtMqttPusher->initPusher(m_ip,m_port,QString("video/%1").arg(m_pushId));

    m_lhtMqttReceiver->LhtSubscribe(QString("video/%1").arg(m_recvId));

    m_lhtMqttReceiver->initReceiver(m_ip,m_port);

    m_lhtMqttReceiver->setCallback(this);
    m_timer = new QTimer;
    connect(m_timer,&QTimer::timeout,[=]{
        read();
    });
    return true;
}
/**
 * @brief  视频是否读取完成
 * @return
 */
bool VedioPlay::isEnd()
{
    return m_end;
}

/**
 * @brief    返回当前帧图像播放时间
 * @return
 */
const qint64 &VedioPlay::pts()
{
    return m_pts;
}

void VedioPlay::free()
{
    // 释放上下文swsContext。
    if(m_swsContext)
    {
        sws_freeContext(m_swsContext);
        m_swsContext = nullptr;             // sws_freeContext不会把上下文置NULL
    }
    if(m_imgConvertCtx)
    {
        sws_freeContext(m_imgConvertCtx);
        m_imgConvertCtx = nullptr;             // sws_freeContext不会把上下文置NULL
    }
    if(m_imgConvertRemoteCtx)
    {
        sws_freeContext(m_imgConvertRemoteCtx);
        m_imgConvertRemoteCtx = nullptr;             // sws_freeContext不会把上下文置NULL
    }
    // 释放编解码器上下文和与之相关的所有内容，并将NULL写入提供的指针
    if(m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }
    // 释放编解码器上下文和与之相关的所有内容，并将NULL写入提供的指针
    // 关闭并失败m_formatContext，并将指针置为null
    if(m_formatContext)
    {
        avformat_close_input(&m_formatContext);
    }
    if(m_packet)
    {
        av_packet_free(&m_packet);
    }
    if(m_frame)
    {
        av_frame_free(&m_frame);
    }
    if(m_frameRGB)
    {
        av_frame_free(&m_frameRGB);
    }
    if(m_frameRemoteRGB)
    {
        av_frame_free(&m_frameRemoteRGB);
    }

//    if(m_buffer)
//    {
//        delete [] m_buffer;
//        m_buffer = nullptr;
//    }
//    if(m_rgbbuffer)
//    {
//        delete []m_rgbbuffer;
//        m_rgbbuffer = nullptr;
//    }
//    if(m_rgbRemoteBuffer)
//    {
//        delete []m_rgbRemoteBuffer;
//        m_rgbRemoteBuffer = nullptr;
//    }
    if(m_encode){
        delete m_encode;
        m_encode = nullptr;
    }
    if(m_decode){
        delete m_decode;
        m_decode = nullptr;
    }
}
/**
 * @brief 关闭视频播放并释放内存
 */
void VedioPlay::close()
{
    m_timer->stop();
    clear();
    free();

    m_totalTime     = 0;
    m_videoIndex    = 0;
    m_totalFrames   = 0;
    m_obtainFrames  = 0;
    m_pts           = 0;
    m_frameRate     = 0;
    m_size          = QSize(0, 0);
}

/**
 * @brief 清空读取缓冲
 */
void VedioPlay::clear()
{
    // 因为avformat_flush不会刷新AVIOContext (s->pb)。如果有必要，在调用此函数之前调用avio_flush(s->pb)。
    if(m_formatContext && m_formatContext->pb)
    {
        avio_flush(m_formatContext->pb);
    }
    if(m_formatContext)
    {
        avformat_flush(m_formatContext);   // 清理读取缓冲
    }
}

bool VedioPlay::lhtReadyRead(const QByteArray& arr)
{
    qint64 mes = 0;
    mes = arr.mid(0,8).toLongLong();
//    qDebug()<<"VedioPlay decode mes = "<<mes;
    if(!m_decode){
        return false;
    }
    auto frame = m_decode->decode(arr.mid(8));
    if(!frame){
        qDebug()<<"decode error";
        return 0;
    }
    sws_scale(m_imgConvertRemoteCtx, (const uint8_t * const *)frame->data, frame->linesize,
              0, m_codecContext->height, m_frameRemoteRGB->data, m_frameRemoteRGB->linesize);
    //本地显示一下
    QImage image(m_frameRemoteRGB->data[0], m_codecContext->width, m_codecContext->height, QImage::Format_RGB888);
    signRemoteImageReadReady(image);
    return 1;
}
/**
 * @brief        显示ffmpeg函数调用异常信息
 * @param err
 */
void VedioPlay::showError(int err)
{
#if PRINT_LOG
    memset(m_error, 0, ERROR_LEN);        // 将数组置零
    av_strerror(err, m_error, ERROR_LEN);
    qWarning() << "VedioPlay Error：" << m_error;
#else
    Q_UNUSED(err)
#endif
}
/**
 * @brief          将AVRational转换为double，用于计算帧率
 * @param rational
 * @return
 */
qreal VedioPlay::rationalToDouble(AVRational* rational)
{
    qreal frameRate = (rational->den == 0) ? 0 : (qreal(rational->num) / rational->den);
    return frameRate;
}
