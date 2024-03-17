#ifndef ONLINE_USER_LIST_HANDLE_H
#define ONLINE_USER_LIST_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class OnlineUserListHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void OnlineUserListHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    qDebug()<<"OnlineUserList";
    QString userList(msg.get()->data);
    GlobalCenter::getInstance()->signOnlineUserList(userList);
}



#endif // LOGIN_HANDLE_H
