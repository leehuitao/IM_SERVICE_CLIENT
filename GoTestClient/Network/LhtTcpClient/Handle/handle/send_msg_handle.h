#ifndef SEND_MSG_HANDLE_H
#define SEND_MSG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class SendMsgHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void SendMsgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    GlobalCenter::getInstance()->slotRecvMsg(body);
}



#endif // LOGIN_HANDLE_H
