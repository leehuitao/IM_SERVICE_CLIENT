#ifndef LHTOPUSENCODE_H
#define LHTOPUSENCODE_H
#include "encode_basic.h"
//语音编码以及解码  多线程版本
#include <QFile>
#include <QObject>
#include "opus/opus.h"

class LhtOpusEncode :public EncodeBasic
{
public:
    LhtOpusEncode(EncodeCallback *callback,LhtCodeType codeType,QObject *parent = nullptr);

    // EncodeBasic interface
public:
    void initEncoder();
    QByteArray encode(char *data, int len);
    QByteArray decode(char *data, int len);

private:
    OpusEncoder *m_encoder = nullptr;
    OpusDecoder *m_decoder = nullptr;
    unsigned int m_frameSize = 960;
    unsigned char m_encodeData[1024];
    opus_int16 m_decodeData[1024];
};

#endif // LHTOPUSENCODE_H
