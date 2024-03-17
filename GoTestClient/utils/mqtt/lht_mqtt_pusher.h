#ifndef MQTTPUSHER_H
#define MQTTPUSHER_H

#include <QObject>
#include <QDateTime>
#include <QtMqtt/qmqttclient.h>

class LhtMqttPusher :public QObject
{
    Q_OBJECT
public:
    LhtMqttPusher();

    void initPusher(QString ip, int port , QString topic);
    void LhtMqttPush(const QByteArray &);

private slots:
    void slotConnected();
    void slotStateChange();
    void slotDisconnected();
private:
    QMqttClient *m_client = nullptr;

    QString  m_topic;

    QString m_ip;

    int m_port;
};

#endif // MQTTPUSHER_H
