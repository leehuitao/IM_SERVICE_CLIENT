#include "lht_mqtt_pusher.h"

LhtMqttPusher::LhtMqttPusher()
{

}

void LhtMqttPusher::initPusher(QString ip, int port, QString topic)
{
    m_ip = ip;
    m_port = port;
    m_topic = topic;
    if(m_client == nullptr){
        m_client = new QMqttClient(this);
        connect(m_client, &QMqttClient::connected, this, &LhtMqttPusher::slotConnected);
        connect(m_client, &QMqttClient::stateChanged, this, &LhtMqttPusher::slotStateChange);
        connect(m_client, &QMqttClient::disconnected, this, &LhtMqttPusher::slotDisconnected);

        connect(m_client, &QMqttClient::pingResponseReceived, this, [&]() {
        });
    }
    m_client->setHostname(ip);
    m_client->setPort(port);
    m_client->connectToHost();


}

void LhtMqttPusher::LhtMqttPush(const QByteArray & data)
{
    if(m_client->state() == QMqttClient::Connected){
        if (m_client->publish(m_topic, data) == -1){
            qDebug() << "Could not publish data";
        }
    }
}

void LhtMqttPusher::slotConnected()
{
    qDebug() << "Connected!";
}

void LhtMqttPusher::slotStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(m_client->state())
            + QLatin1Char('\n');
    qDebug() << content;
}

void LhtMqttPusher::slotDisconnected()
{
    qDebug() << "Disconnected!";
}
