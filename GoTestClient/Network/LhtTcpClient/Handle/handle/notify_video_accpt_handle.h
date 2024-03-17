#ifndef NOTIFY_VIDEO_ACCPT_HANDLE_H
#define NOTIFY_VIDEO_ACCPT_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class NotifyVideoAccptHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void NotifyVideoAccptHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    if(body.UserId != AppCache::Instance()->m_userId){//我接收的
        GlobalCenter::getInstance()->slotAccptVideo(body);
    }
}



#endif // LOGIN_HANDLE_H
