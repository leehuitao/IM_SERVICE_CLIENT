#include "lht_mqtt_receiver.h"

LhtMqttReceiver::LhtMqttReceiver()
{

}

void LhtMqttReceiver::initReceiver(QString ip, int port)
{
    m_ip = ip;
    m_port = port;
    if(m_client == nullptr){
        m_client = new QMqttClient(this);
        connect(m_client, &QMqttClient::connected, this, &LhtMqttReceiver::slotConnected);
        connect(m_client, &QMqttClient::stateChanged, this, &LhtMqttReceiver::slotStateChange);
        connect(m_client, &QMqttClient::disconnected, this, &LhtMqttReceiver::slotDisconnected);

        connect(m_client, &QMqttClient::pingResponseReceived, this, [&]() {
        });
    }
    m_client->setHostname(m_ip);
    m_client->setPort(m_port);
    m_client->connectToHost();
}


void LhtMqttReceiver::slotReceiveMess(const QByteArray & data, const QMqttTopicName &)
{
    if(m_callback)
        m_callback->lhtReadyRead(data);
//    qDebug()<<"mqtt recvData len = "<<data.length();
}


void LhtMqttReceiver::LhtSubscribe(QString topic)
{
    m_topic.append(topic);
}


void LhtMqttReceiver::slotConnected()
{
    qDebug() << "Connected!";
    if(m_client->state() == QMqttClient::Connected){
        for(auto it : m_topic){
            auto subscription = m_client->subscribe(it, 0);
            if (!subscription) {
                qDebug() << "Could not subscribe. Is there a valid connection?";
                return;
            }
        }
        connect(m_client, SIGNAL(messageReceived(QByteArray,QMqttTopicName)), this, SLOT(slotReceiveMess(QByteArray,QMqttTopicName)));
    }
}

void LhtMqttReceiver::slotStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
            + QLatin1String(": State Change")
            + QString::number(m_client->state())
            + QLatin1Char('\n');
    qDebug() << content;
}

void LhtMqttReceiver::slotDisconnected()
{
    qDebug() << "Disconnected!";
}

