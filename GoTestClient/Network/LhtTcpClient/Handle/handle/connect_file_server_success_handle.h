#ifndef CONENCT_FILE_SERVER_SUCCESS_HANDLE_H
#define CONENCT_FILE_SERVER_SUCCESS_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class ConnectFileServerSuccessHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void ConnectFileServerSuccessHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    qDebug()<<"send and recv connect success!!!";
    //通知接收端准备接收数据
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    if(body.RecvUserId == AppCache::Instance()->m_userId)
        GlobalCenter::getInstance()->signStartRecvFile(body);
    if(body.SendUserId == AppCache::Instance()->m_userId)
        GlobalCenter::getInstance()->signStartSendFile(body);
}



#endif // LOGIN_HANDLE_H
