#ifndef VEDIOPLAY_H
#define VEDIOPLAY_H

#include <QImage>
#include <QObject>
#include <QSize>
#include <QMutex>
#include "Encode/x264_encode.h"
#include "Encode/x264_decode.h"
#include "utils/mqtt/lht_mqtt_pusher.h"
#include "utils/mqtt/lht_mqtt_receiver.h"
class VedioPlay : public QObject,public LhtCallback
{
    Q_OBJECT

public:
    explicit VedioPlay(QObject *parent = nullptr);

    void initFFmpeg();

public slots:
    bool open(const QString& url ,QString ip , int port,int pushId,int recvId);    // 打开媒体文件，或者流媒体rtmp、strp、http
    AVFrame* read();                               // 读取视频图像
    void startCommunication();                      // 与对方建立连接成功  开始通信
    void close();                                 // 关闭
    const qint64 &pts();
    bool isEnd();
signals:
    void signImageReadReady(QImage);
    void signRemoteImageReadReady(QImage);
private:
    const AVInputFormat* m_inputFormat = nullptr;
    AVFormatContext* m_formatContext = nullptr;   // 解封装上下文
    AVCodecContext*  m_codecContext  = nullptr;   // 解码器上下文
    SwsContext*      m_swsContext    = nullptr;   // 图像转换上下文
    SwsContext*      m_imgConvertCtx = nullptr;   // 图像转换上下文
    SwsContext*      m_imgConvertRemoteCtx = nullptr;   // 图像转换上下文

    AVPacket* m_packet = nullptr;                 // 数据包
    AVFrame*  m_frame  = nullptr;                 // 解码后的视频帧
    AVFrame*  m_frameRGB  = nullptr;                 // 解码后的RGB视频帧
    AVFrame*  m_frameRemoteRGB  = nullptr;                 // 解码后远程的RGB视频帧
    int    m_videoIndex   = 0;                    // 视频流索引
    qint64 m_totalTime    = 0;                    // 视频总时长
    qint64 m_totalFrames  = 0;                    // 视频总帧数
    qint64 m_obtainFrames = 0;                    // 视频当前获取到的帧数
    qint64 m_pts          = 0;                    // 图像帧的显示时间
    qreal  m_frameRate    = 0;                    // 视频帧率
    QSize  m_size;                                // 视频分辨率大小
    char*  m_error = nullptr;                     // 保存异常信息
    bool   m_end = false;                         // 视频读取完成
    uchar* m_buffer = nullptr;                    // YUV图像需要转换位RGBA图像，这里保存转换后的图形数据
    uint8_t *m_rgbbuffer = nullptr;
    uint8_t *m_rgbRemoteBuffer = nullptr;
    X264Encode * m_encode;
    X264Decode * m_decode;

    QTimer          * m_timer;
    int m_pushId;

    int m_recvId;

    QString m_ip;

    int m_port;
    LhtMqttPusher *m_lhtMqttPusher;

    LhtMqttReceiver *m_lhtMqttReceiver;

    bool m_startCommunication = false;

    QMutex m_mutex;
private:
    void free();
    void showError(int err);
    qreal rationalToDouble(AVRational *rational);
    void clear();


    // LhtCallback interface
public:
    bool lhtReadyRead(const QByteArray& arr);
};

#endif // VEDIOPLAY_H
