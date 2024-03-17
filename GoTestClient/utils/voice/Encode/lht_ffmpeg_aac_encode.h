#ifndef LHTFFMPEGAACENCODE_H
#define LHTFFMPEGAACENCODE_H
#include "encode_basic.h"
//语音编码以及解码   多线程版本
#include <QFile>
#include <QObject>
#if defined(__cplusplus)
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>

#if defined(__cplusplus)
}
#endif
class LhtFFmpegAacEncode:public EncodeBasic
{
public:
    LhtFFmpegAacEncode(EncodeCallback *callback,LhtCodeType codeType,QObject *parent = nullptr);

    // EncodeBasic interface
public:
    void initEncoder();
    QByteArray encode(char *data, int len);
    QByteArray decode(char *data, int len);

private:
    AVFrame* m_inputFrame;
    AVCodecContext* m_encoderCtx;
    AVCodecContext *m_decodecCtx;
    AVPacket * m_encodePacket;
    AVPacket * m_decodePacket;
    AVFrame * m_decodeFrame;
    std::ofstream *outFile;
    std::ofstream *outDeFile;
    QByteArray encodeAudioFrame();
};

#endif // LHTFFMPEGAACENCODE_H
