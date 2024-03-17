#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H
#include <QAudioInput>
#include <QIODevice>
#include <QTimer>
#include <iostream>
#include <string>
#include "utils/encode/lht_opus_encode.h"
#include "utils/encode/lht_ffmpeg_aac_encode.h"
#include "utils/mqtt/lht_mqtt_pusher.h"
#include "utils/mqtt/lht_mqtt_receiver.h"
#include "utils/encode/aac_encode.h"
#include "utils/encode/opus_encode.h"

class AudioInput : public QObject ,public EncodeCallback,public LhtCallback
{
    Q_OBJECT

public:
    AudioInput( QObject *parent = nullptr) ;
public slots:
    void startRead(QString ip , int port,int pushId,int recvId);

    void setPause(bool pause);

    void stopRead();
    void setCallback(LhtCallback * c){
        m_lhtCallback = c;
    }
     void initEncoder();
signals:
    void signSendData(const QByteArray &);
    // EncodeCallback interface
public:
    void onEncodeFrame(QByteArray) override;

    void onDecodeFrame(QByteArray) override;

private:

    void createOutput();

    void createIntput();
private:
    bool m_stop = 1;

    bool m_pause = 0;

    int m_pushId;

    int m_recvId;

    QString m_ip;

    int m_port;

    QTimer *timer;;

    QByteArray audioDataCache;

    QIODevice *m_audioInput= nullptr;

    QIODevice * m_audioOutput= nullptr;

    LhtOpusEncode *m_lhtOpusEncoder= nullptr;

    LhtOpusEncode *m_lhtOpusDecoder= nullptr;

    LhtFFmpegAacEncode *m_lhtFfmpegAacEncoder= nullptr;

    LhtFFmpegAacEncode *m_lhtFfmpegAacDecoder= nullptr;

    LhtCallback * m_lhtCallback = nullptr;

    LhtMqttPusher *m_lhtMqttPusher;

    LhtMqttReceiver *m_lhtMqttReceiver;
    //---------------测试------------------
    AacEncode *m_aacEncode;

    OpusEncode *m_opuEncode;

    AacEncode *m_aacDecode;

    OpusEncode *m_opuDecode;


    //---------------测试------------------
    // LhtCallback interface
public:
    bool lhtReadyRead(const QByteArray& arr) override;
};

#endif // AUDIOPLAY_H
