#include "audio_play.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include <QIODevice>
#include <QTimer>

#define TestOpus
#ifndef TestOpus
#define TestAAC
#endif
#ifdef  TestOpus
#define TestSampleRate  24000
#define FRAME_SIZE 960 // 每个帧中PCM样本数
#endif

#ifdef TestAAC
#define TestSampleRate  44100
#define FRAME_SIZE 2048 // 每个帧中PCM样本数
#endif
//Mqtt测试只写了opus编码，如果要测试这个上面#define TestOpus一定要打开
#define MqttTest



AudioInput::AudioInput(QObject *parent)
{



}

void AudioInput::startRead(QString ip , int port,int pushId,int recvId)
{
    m_ip = ip;
    m_port = port;
    m_pushId = pushId;
    m_recvId = recvId;
    initEncoder();
    m_pause = 0;
    m_stop = 0;
    createIntput();
    createOutput();
    timer->start(20); // 定时器每 1000 毫秒触发一次，即每秒触发一次
}

void AudioInput::setPause(bool pause)
{
    m_pause = pause;
}

void AudioInput::stopRead()
{
    timer->stop();
    m_audioInput->close();
    m_audioOutput->close();
    m_stop = 1;
}

void AudioInput::initEncoder()
{
    m_aacEncode = new AacEncode(LhtEncode);

    m_opuEncode = new OpusEncode(LhtEncode);

    m_aacDecode = new AacEncode(LhtDecode);

    m_opuDecode = new OpusEncode(LhtDecode);

    m_aacEncode->initEncoder();
    m_opuEncode->initEncoder();
    m_aacDecode->initEncoder();
    m_opuDecode->initEncoder();

    m_lhtMqttPusher = new LhtMqttPusher;

    m_lhtMqttReceiver = new LhtMqttReceiver;

    m_lhtMqttPusher->initPusher(m_ip,m_port,QString("test/%1").arg(m_pushId));

    m_lhtMqttReceiver->LhtSubscribe(QString("test/%1").arg(m_recvId));

    m_lhtMqttReceiver->initReceiver(m_ip,m_port);


    m_lhtMqttReceiver->setCallback(this);
    //多线程版本的opus编码
    m_lhtOpusEncoder = new LhtOpusEncode(this,LhtEncode,nullptr);
    m_lhtOpusEncoder->initEncoder();
    m_lhtOpusDecoder = new LhtOpusEncode(this,LhtDecode,nullptr);
    m_lhtOpusDecoder->initEncoder();
    //多线程版本的AAC编码
    m_lhtFfmpegAacEncoder = new LhtFFmpegAacEncode(this,LhtEncode,nullptr);
    m_lhtFfmpegAacEncoder->initEncoder();
    m_lhtFfmpegAacDecoder = new LhtFFmpegAacEncode(this,LhtDecode,nullptr);
    m_lhtFfmpegAacDecoder->initEncoder();

    timer = new QTimer;


    QObject::connect(timer, &QTimer::timeout, [&]() {
        if(m_pause)
            return ;
        QByteArray audioData = m_audioInput->readAll();
        audioDataCache.append(audioData);
        while(!m_pause && audioDataCache.size() >= FRAME_SIZE * 2){
            QByteArray arr = audioDataCache.mid(0,FRAME_SIZE* 2);
            auto mes = QDateTime::currentMSecsSinceEpoch();
            QByteArray pushData;
            pushData.append((char*)&mes,8);
            pushData.append(m_opuEncode->encode(arr.data(),arr.size()));
#ifndef MqttTest
#ifdef  TestOpus
            m_lhtOpusEncoder->recvData(arr);
#else
            m_lhtFfmpegAacEncoder->recvData(arr);
#endif
#else
            m_lhtMqttPusher->LhtMqttPush(pushData);
#endif
            audioDataCache.remove(0,FRAME_SIZE* 2);
        }
    });
}

bool AudioInput::lhtReadyRead(const QByteArray &encodeData)
{
#ifndef MqttTest
    #ifdef  TestOpus
        m_lhtOpusDecoder->recvData(encodeData.data(),encodeData.length());
    #else
        m_lhtFfmpegAacDecoder->recvData(encodeData.data(),encodeData.length());
    #endif
#else
    qint64 mes = 0;
    mes = encodeData.mid(0,8).toLongLong();
    qDebug()<<"AudioInput decode mes = "<<mes;
    auto ret = m_opuDecode->decode(encodeData.mid(8).constData(),encodeData.mid(8).size());
    onDecodeFrame(ret);
#endif
    return 1;
}

void AudioInput::onEncodeFrame(QByteArray encodeData)
{
#ifndef MqttTest
#ifdef  TestOpus
    m_lhtCallback->lhtReadyRead(encodeData);
    //m_lhtOpusDecoder->recvData(encodeData.data(),encodeData.length());
#else
    m_lhtCallback->lhtReadyRead(encodeData);
    //m_lhtFfmpegAacDecoder->recvData(encodeData.data(),encodeData.length());

#endif
#else

#endif

}

void AudioInput::onDecodeFrame(QByteArray decodeData)
{
    m_audioOutput->write(decodeData);
}


void AudioInput::createOutput()
{
    // 创建音频设备对象
    QAudioFormat format;
    format.setSampleRate(TestSampleRate);
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

void AudioInput::createIntput()
{
    // 获取音频输入设备信息
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if (devices.isEmpty()) {
        qDebug() << "No audio input devices available";
        return;
    }

    // 配置音频输入参数
    QAudioFormat format;
    format.setSampleRate(TestSampleRate);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    /* 获取默认音频设备 */
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();

    /* 指定音频设备和参数构造QAudioInput实例 */
    QAudioInput* audio = new QAudioInput(device, format);
    audio->setBufferSize(TestSampleRate * 2); // 设置缓冲区大小为 88200 字节

    m_audioInput = audio->start();
}
