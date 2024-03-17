#ifndef UPDATE_ONLINE_USER_HANDLE_H
#define UPDATE_ONLINE_USER_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UpdateOnlineUserHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UpdateOnlineUserHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    qDebug()<<"UpdateOnlineUser";
    QString userList(msg.get()->data);
    OnlineListBody body;
    body = PacketProcess::parseOnlineListBodyPack(msg.get()->data);
    GlobalCenter::getInstance()->slotOnlineUserUpdate(body);
}



#endif // LOGIN_HANDLE_H
