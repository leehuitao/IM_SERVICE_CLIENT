#include "my_mqtt_client.h"

MyMQTTClient::MyMQTTClient(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setHostname("192.168.1.7");
    m_client->setPort(1883);
    m_client->connectToHost();

    connect(m_client, &QMqttClient::connected, this, &MyMQTTClient::brokerConnected);
    connect(m_client, &QMqttClient::stateChanged, this, &MyMQTTClient::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MyMQTTClient::brokerDisconnected);

    connect(m_client, &QMqttClient::pingResponseReceived, this, [this]() {
        const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(" PingResponse")
                    + QLatin1Char('\n');
        qDebug() << content;
    });
}

void MyMQTTClient::MyMQTTSubscribe(QString str)
{
    auto subscription = m_client->subscribe(str, 0);
    if (!subscription) {
        qDebug() << "Could not subscribe. Is there a valid connection?";
        return;
    }
}

void MyMQTTClient::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    qDebug() << content;
}

void MyMQTTClient::brokerConnected()
{
    qDebug() << "Connected!";
    if(m_client->state() == QMqttClient::Connected){
        QMqttTopicFilter topic("audio/roomid=1");
        m_client->subscribe(topic, 0);
        connect(m_client, SIGNAL(messageReceived(QByteArray,QMqttTopicName)), this, SLOT(receiveMess(QByteArray,QMqttTopicName)));
    }
}

void MyMQTTClient::brokerDisconnected()
{
    qDebug() << "Disconnected!";
}

void MyMQTTClient::receiveMess(const QByteArray &message, const QMqttTopicName &topic)
{
   QString content;
   content = QDateTime::currentDateTime().toString() + QLatin1Char('\n');
   content += QLatin1String(" Received Topic: ") + topic.name() + QLatin1Char('\n');
   content += QLatin1String(" Message: ") + message + QLatin1Char('\n');
   qDebug() << content;
   recvData(message);
}

void MyMQTTClient::MyMQTTSendMessage(const QString topic, const QString message)
{
    if (m_client->publish(topic, message.toUtf8()) == -1){
        qDebug() << "Could not publish message";
    }
}
