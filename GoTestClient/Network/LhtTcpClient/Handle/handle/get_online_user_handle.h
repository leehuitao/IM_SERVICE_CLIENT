#ifndef GET_ONLINE_USER_HANDLE_H
#define GET_ONLINE_USER_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetOnlineUserHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetOnlineUserHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    QLOG_INFO()<<"OnlineUserList";
    QString userList(msg.get()->data);
    GlobalCenter::getInstance()->signOnlineUserList(userList);
}



#endif // LOGIN_HANDLE_H
