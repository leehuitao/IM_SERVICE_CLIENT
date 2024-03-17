#ifndef OPUSENCODE_H
#define OPUSENCODE_H

//语音编码以及解码
#include <QFile>
#include <QObject>
#include "opus/opus.h"
#include "utils/lht_define.h"

class OpusEncode : public QObject
{
    Q_OBJECT
public:
    explicit OpusEncode(LhtCodeType codeType,QObject *parent = nullptr);
    // EncodeBasic interface
public:
    void initEncoder();
    QByteArray encode(char *data, int len);
    QByteArray decode(const char *data, int len);

private:
    OpusEncoder *m_encoder = nullptr;
    OpusDecoder *m_decoder = nullptr;
    unsigned int m_frameSize = 960;
    unsigned char m_encodeData[1024];
    opus_int16 m_decodeData[1024];
    int32_t         m_sample = 24000;//采样率
    int32_t         m_channel = 2;//通道数
    int32_t         m_sampleSize = 16;//采样大小
    LhtCodeType      m_codeType;
};

#endif // OPUSENCODE_H
