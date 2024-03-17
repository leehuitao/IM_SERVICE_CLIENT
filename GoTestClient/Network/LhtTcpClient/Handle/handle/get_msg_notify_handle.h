#ifndef GET_MSG_NOTIFY_HANDLE_H
#define GET_MSG_NOTIFY_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetMsgNotifyHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetMsgNotifyHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body =  PacketProcess::parseMsgPack(msg.get()->data);
    GlobalCenter::getInstance()->slotRecvMsgNotify(body);
}



#endif // LOGIN_HANDLE_H
