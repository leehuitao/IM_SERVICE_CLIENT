#ifndef MQTTRECEIVER_H
#define MQTTRECEIVER_H

#include <QObject>
#include <QDateTime>
#include <QtMqtt/qmqttclient.h>
#include "utils/lht_callback.h"

class LhtMqttReceiver :public QObject
{
    Q_OBJECT
public:
    LhtMqttReceiver();

    void initReceiver(QString ip, int port);

    void LhtSubscribe(QString);

    void setCallback(LhtCallback *callback){
        m_callback = callback;
    }
private slots:
    void slotConnected();
    void slotStateChange();
    void slotDisconnected();
    void slotReceiveMess(const QByteArray &, const QMqttTopicName &);
private:
    QMqttClient *m_client = nullptr;

    QStringList  m_topic;

    QString m_ip;

    int m_port;

    LhtCallback *m_callback = nullptr;
};

#endif // MQTTRECEIVER_H
