#ifndef UPDATE_MSG_STATUS_HANDLE_H
#define UPDATE_MSG_STATUS_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UpdateMsgStatusHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UpdateMsgStatusHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    if(body.DstUserId != AppCache::Instance()->m_userId){
        GlobalCenter::getInstance()->slotUpdateMsgStatus(body);
    }
}



#endif // LOGIN_HANDLE_H
