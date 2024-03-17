#ifndef NOTIFY_AUDIO_ACCPT_HANDLE_H
#define NOTIFY_AUDIO_ACCPT_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class NotifyAudioAccptHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void NotifyAudioAccptHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    MsgBody body;
    body = PacketProcess::parseMsgPack(msg.get()->data);
    if(body.UserId != AppCache::Instance()->m_userId){//我接收的
        GlobalCenter::getInstance()->slotAccptAudio(body);
    }
}



#endif // LOGIN_HANDLE_H
