#ifndef SEND_GROUP_MSG_HANDLE_H
#define SEND_GROUP_MSG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class SendGroupMsgHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void SendGroupMsgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    GroupBody body;
    body = PacketProcess::parseGroupBodyPack(msg.get()->data);
    if(AppCache::Instance()->m_userId == body.UserId){//发消息的返回包

    }else{
        GlobalCenter::getInstance()->slotNewGroupMsg(body);
    }

}
#endif // SEND_GROUP_MSG_HANDLE_H
