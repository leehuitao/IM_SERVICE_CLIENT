#ifndef GROUP_INVATATION_HANDLE_H
#define GROUP_INVATATION_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GroupInvitationHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GroupInvitationHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    GroupBody body;
    body = PacketProcess::parseGroupBodyPack(msg.get()->data);
    if(AppCache::Instance()->m_userId == body.UserId){
        GlobalCenter::getInstance()->slotCreateGroup(body);
    }else{

    }

}

#endif // GROUP_INVATATION_HANDLE_H
