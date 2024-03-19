#ifndef CREATE_GROUP_HANDLE_H
#define CREATE_GROUP_HANDLE_H

#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class CreateGroupHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void CreateGroupHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    GroupBody body;
    body = PacketProcess::parseGroupBodyPack(msg.get()->data);
    if(AppCache::Instance()->m_userId == body.UserId){//创建的返回包

    }else{

    }

}

#endif // CREATE_GROUP_HANDLE_H
