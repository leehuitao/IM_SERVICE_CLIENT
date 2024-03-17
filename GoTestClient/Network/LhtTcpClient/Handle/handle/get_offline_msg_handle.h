#ifndef GET_OFFLINE_MSG_HANDLE_H
#define GET_OFFLINE_MSG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetOfflineMsgHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetOfflineMsgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    GlobalCenter::getInstance()->slotGetOfflineNotify(body);
}



#endif // LOGIN_HANDLE_H
