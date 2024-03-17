#include "x264_encode.h"

#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QDateTime>

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

X264Encode::X264Encode(LhtCodeType codeType, QObject *parent): QObject(parent)
{

}

void X264Encode::initEncoder()
{
    /* find the mpeg1video encoder */
    auto enc = avcodec_find_encoder_by_name("libx264");
    if (!enc) {
        fprintf(stderr, "Codec '%s' not found\n", "libx264");
        exit(1);
    }

    m_encCtx = avcodec_alloc_context3(enc);
    if (!m_encCtx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    m_pkt = av_packet_alloc();
    if (!m_pkt)
        exit(1);

    /* put sample parameters */
    m_encCtx->bit_rate = 100000;
    /* resolution must be a multiple of two */
    m_encCtx->width = 640;
    m_encCtx->height = 480;
    /* frames per second */
    m_encCtx->time_base = {1, 25};
    m_encCtx->framerate = {25, 1};

    /* emit one intra frame every ten frames
         * check frame pict_type before passing frame
         * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
         * then gop_size is ignored and the output of encoder
         * will always be I frame irrespective to gop_size
         */
    m_encCtx->gop_size = 0;
    m_encCtx->max_b_frames = 0;
    m_encCtx->pix_fmt = AV_PIX_FMT_YUVJ422P;

    if (enc->id == AV_CODEC_ID_H264)
        av_opt_set(m_encCtx->priv_data, "preset", "slow", 0);

    /* open it */
    auto ret = avcodec_open2(m_encCtx, enc, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %d\n", ret);
        exit(1);
    }

}

AVPacket *  X264Encode::encode(AVFrame *frame)
{
    int ret;
    //    av_packet_unref(m_pkt);
    /* send the frame to the encoder */
    //    if (frame)
    //        printf("Send frame %3""lld""\n", frame->pts);

    ret = avcodec_send_frame(m_encCtx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(m_encCtx, m_pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return nullptr;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        //        printf("Write packet %3""lld"" (size=%5d)\n", m_pkt->pts, m_pkt->size);
        return m_pkt;
        //        av_packet_unref(m_pkt);
    }
}

QByteArray X264Encode::decode(char *data, int len)
{
    return QByteArray();
}
