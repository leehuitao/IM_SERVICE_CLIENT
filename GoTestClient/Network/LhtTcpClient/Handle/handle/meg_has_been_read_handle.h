#ifndef MSG_HAS_BEEN_READ_HANDLE_H
#define MSG_HAS_BEEN_READ_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class MsgHasBeenReadHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void MsgHasBeenReadHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    GlobalCenter::getInstance()->slotUpdateMsgStatus(body);
}



#endif // LOGIN_HANDLE_H
