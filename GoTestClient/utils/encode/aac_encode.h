#ifndef AACENCODE_H
#define AACENCODE_H

//语音编码以及解码   多线程版本
#include <QFile>
#include <QObject>
#include "utils/lht_define.h"
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

class AacEncode : public QObject
{
    Q_OBJECT
public:
    explicit AacEncode(LhtCodeType codeType,QObject *parent = nullptr);

public:
    void initEncoder();
    QByteArray encode(char *data, int len);
    QByteArray decode(char *data, int len);
private:
    QByteArray encodeAudioFrame();
private:
    AVFrame* m_inputFrame;
    AVCodecContext* m_encoderCtx;
    AVCodecContext *m_decodecCtx;
    AVPacket * m_encodePacket;
    AVPacket * m_decodePacket;
    AVFrame * m_decodeFrame;
    std::ofstream *outFile;
    std::ofstream *outDeFile;
    int32_t         m_sample = 24000;//采样率
    int32_t         m_channel = 2;//通道数
    int32_t         m_sampleSize = 16;//采样大小
    LhtCodeType      m_codeType;
};

#endif // AACENCODE_H
