#ifndef ENCODEBASIC_H
#define ENCODEBASIC_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <iostream>
#include <string>
#include "utils/thread_pool_basic.h"
#include "utils/lht_define.h"
/**
  @brief 需要调用编码时请继承这个类，用于数据回调。
  @author leehuitao
*/
class EncodeCallback {

public:
    virtual void onEncodeFrame(QByteArray) = 0;
    virtual void onDecodeFrame(QByteArray) = 0;
};



class EncodeBasic : public ThreadPoolBasic
{
    Q_OBJECT
public:
    explicit EncodeBasic(EncodeCallback *callback,LhtCodeType codeType,QObject *parent = nullptr);
    void setInfo(int32_t sample,int32_t channel,int32_t sampleSize){
        m_sample = sample;
        m_channel = channel;
        m_sampleSize = sampleSize;
    }
    virtual void initEncoder() = 0;
    virtual QByteArray encode(char * data, int len) = 0;
    virtual QByteArray decode(char * data, int len) = 0;
    void playVoice(QByteArray data);

signals:


protected:
    QIODevice *     m_audioOutput = nullptr;
    int32_t         m_sample = 24000;//采样率
    int32_t         m_channel = 2;//通道数
    int32_t         m_sampleSize = 16;//采样大小
    LhtCodeType      m_codeType;
    EncodeCallback * m_callback = nullptr;
    // ThreadPoolBasic interface
protected:
    void handleData(char *arr, int len);
};

#endif // ENCODEBASIC_H
