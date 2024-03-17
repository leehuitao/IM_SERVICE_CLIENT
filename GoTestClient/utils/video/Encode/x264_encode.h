#ifndef X264ENCODE_H
#define X264ENCODE_H

#include <QObject>
#include "utils/lht_define.h"

struct AVFormatContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVCodec;
struct AVFrame;
struct SwsContext;
struct AVBufferRef;
struct AVInputFormat;
class QImage;

class X264Encode : public QObject
{
    Q_OBJECT
public:
    explicit X264Encode(LhtCodeType codeType,QObject *parent = nullptr);
    void initEncoder();
    AVPacket *  encode(AVFrame *frame);
    QByteArray decode(char *data, int len);
signals:
private:

    LhtCodeType      m_codeType;
    AVCodec*            m_enc;
    AVCodecContext *    m_encCtx;
    AVPacket *          m_pkt;
};

#endif // X264ENCODE_H
