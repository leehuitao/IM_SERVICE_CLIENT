#ifndef UPDATE_GROUP_MSG_STATE_HANDLE_H
#define UPDATE_GROUP_MSG_STATE_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UpdateGroupMsgStateHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void UpdateGroupMsgStateHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    GroupBody body;
    body = PacketProcess::parseGroupBodyPack(msg.get()->data);

}
#endif // UPDATE_GROUP_MSG_STATE_HANDLE_H
