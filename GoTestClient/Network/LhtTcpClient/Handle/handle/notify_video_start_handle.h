#ifndef NOTIFY_VIDEO_START_HANDLE_H
#define NOTIFY_VIDEO_START_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class NotifyVideoStartHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void NotifyVideoStartHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    if(body.UserId == AppCache::Instance()->m_userId){//我发出的
        if(body.MsgStatus == -1){
            QLOG_ERROR()<<QStringLiteral("对方不在线");
        }
    }else{
        GlobalCenter::getInstance()->slotStartVideo(body);
    }
}



#endif // LOGIN_HANDLE_H
