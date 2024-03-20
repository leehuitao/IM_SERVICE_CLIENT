#ifndef GET_GROUP_MSG_HANDLE_H
#define GET_GROUP_MSG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetGroupMsgHandle
{
public:
    void HandleMessage(NewTcpProtocolPtr msg);

};

void GetGroupMsgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    GroupBody body;
    body = PacketProcess::parseGroupBodyPack(msg.get()->data);
    GlobalCenter::getInstance()->slotNewGroupMsg(body);
}
#endif // GET_GROUP_MSG_HANDLE_H
