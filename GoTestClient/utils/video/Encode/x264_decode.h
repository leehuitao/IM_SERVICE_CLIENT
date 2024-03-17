#ifndef X264_DECODE_H
#define X264_DECODE_H

#include <QObject>
extern "C"{
#include "libavcodec/defs.h"
}
struct AVFormatContext;
struct AVCodecParserContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVCodec;
struct AVFrame;
struct SwsContext;
struct AVBufferRef;
struct AVInputFormat;
class QImage;
class QMutex;

#define INBUF_SIZE 4096

class X264Decode : public QObject
{
    Q_OBJECT
public:
    explicit X264Decode(QObject *parent = nullptr);

    void initDecoder();

    AVFrame * decode(AVPacket *  pkt);
    AVFrame * decode(const QByteArray &);
signals:

private:
    AVCodecParserContext *m_parser;
    AVCodecContext * m_c= NULL;
    AVFrame *   m_frame;
    uint8_t     m_inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    AVPacket *  m_pkt;
    QMutex   *  m_mutex;
};

#endif // X264_DECODE_H
