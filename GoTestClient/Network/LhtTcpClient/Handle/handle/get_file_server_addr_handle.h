#ifndef GET_FILE_SERVER_ADDR_HANDLE_H
#define GET_FILE_SERVER_ADDR_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetFileServerAddrHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetFileServerAddrHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    QString ipPort = QString(msg.get()->data);

    GlobalCenter::getInstance()->signFileServer(ipPort.split(":").first(),ipPort.split(":").last().toInt());
}



#endif // LOGIN_HANDLE_H
