#ifndef NOTIFY_AUDIO_START_HANDLE_H
#define NOTIFY_AUDIO_START_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class NotifyAudioStartHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void NotifyAudioStartHandle::HandleMessage(NewTcpProtocolPtr msg)
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
        GlobalCenter::getInstance()->slotStartAudio(body);
    }
}



#endif // LOGIN_HANDLE_H
