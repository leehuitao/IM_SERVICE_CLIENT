#ifndef GET_MSG_HANDLE_H
#define GET_MSG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetMsgHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetMsgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
}



#endif // LOGIN_HANDLE_H
