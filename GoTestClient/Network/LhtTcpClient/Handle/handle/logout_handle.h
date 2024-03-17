#ifndef LOGOUT_HANDLE_H
#define LOGOUT_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class LogoutHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void LogoutHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    GlobalCenter::getInstance()->slotLoginStatus(0,"连接断开");
}



#endif // LOGIN_HANDLE_H
