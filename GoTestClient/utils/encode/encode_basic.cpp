#include "encode_basic.h"
#include <QAudioFormat>
EncodeBasic::EncodeBasic(EncodeCallback *callback, LhtCodeType codeType,  QObject *parent) :
    ThreadPoolBasic(parent),m_codeType(codeType),m_callback(callback)
{
    startRun();

}

void EncodeBasic::playVoice(QByteArray data)
{
    if(m_audioOutput == nullptr){
        // 创建音频设备对象
        QAudioFormat format;
        format.setSampleRate(24000);
        format.setChannelCount(2);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::SignedInt);

        QAudioOutput *audioOutput = new QAudioOutput(format);

        // 打开音频设备
        audioOutput->start();

        // 将音频数据写入音频设备进行播放
        m_audioOutput = audioOutput->start();
    }
    m_audioOutput->write(data);
}

void EncodeBasic::handleData(char *arr, int len)
{
    if(m_callback == nullptr){
        return;
    }
    if(m_codeType == LhtEncode){
        m_callback->onEncodeFrame(encode(arr,len));
    }else if(m_codeType == LhtDecode){
        m_callback->onDecodeFrame(decode(arr,len));
    }
}
