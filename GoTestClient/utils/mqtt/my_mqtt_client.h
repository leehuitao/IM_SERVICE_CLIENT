#ifndef MY_MQTT_CLIENT_H
#define MY_MQTT_CLIENT_H

#include <QObject>
#include <QDateTime>
#include <QtMqtt/qmqttclient.h>

namespace Ui {
class MyMQTTClient;
}

using namespace std;

class MyMQTTClient : public QObject
{
    Q_OBJECT
public:
    explicit MyMQTTClient(QObject *parent = nullptr);
    ~MyMQTTClient(){
    };

    QMqttClient *m_client = nullptr;

    void MyMQTTSubscribe(QString);
    void MyMQTTSendMessage(const QString, const QString);

signals:
    void recvData(QByteArray da);
public slots:
    void brokerConnected();
    void updateLogStateChange();
    void brokerDisconnected();
    void receiveMess(const QByteArray &, const QMqttTopicName &);

private:

};

#endif // MY_MQTT_CLIENT_H
