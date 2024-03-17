#ifndef OPUSCODE_H
#define OPUSCODE_H

#include <QObject>
#include "utils/thread_pool_basic.h"
#include "utils/utils_define.h"
#include "opus.h"
class OpusCode : public ThreadPoolBasic
{
    Q_OBJECT
public:
    explicit OpusCode(CodeType type);

    void initEncoder();

    void initDecoder();

    void encode(QByteArray &arr);

    void decode(QByteArray &arr);
signals:
    void signEncodeData(QByteArray);

    void signDecodeData(QByteArray);
    // ThreadPoolBasic interface
protected:
    void handleData(QByteArray arr, int type);

private:
    int             m_sampleRate = 48000;
    int             m_channelNum = 2;
    CodeType        m_type;
    OpusEncoder *   m_encoder;
    OpusDecoder *   m_decoder;
};

#endif // OPUSCODE_H
