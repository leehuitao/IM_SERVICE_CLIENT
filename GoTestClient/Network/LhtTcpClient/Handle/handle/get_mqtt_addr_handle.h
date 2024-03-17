#ifndef GET_MQTT_ADDR_HANDLE_H
#define GET_MQTT_ADDR_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetMqttAddrHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetMqttAddrHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    QString mqttAddr = msg.get()->data;
    QLOG_INFO()<<"MQTT SERVER "<<mqttAddr;
    //2.发送通知到对方
    GlobalCenter::getInstance()->sendAudioMsg(mqttAddr);
}



#endif // LOGIN_HANDLE_H
