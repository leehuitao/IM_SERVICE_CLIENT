#ifndef UPDATE_CONNECT_FILE_SERVER_STATE_HANDLE_H
#define UPDATE_CONNECT_FILE_SERVER_STATE_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UpdateConnectFileServerStateHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UpdateConnectFileServerStateHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    body.UserId = AppCache::Instance()->m_userId;
    if(body.RecvUserId == AppCache::Instance()->m_userId)
        GlobalCenter::getInstance()->signGetFileAddr(body,0,0);
}



#endif // LOGIN_HANDLE_H
