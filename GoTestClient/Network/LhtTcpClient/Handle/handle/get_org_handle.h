#ifndef GET_ORG_HANDLE_H
#define GET_ORG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetOrgHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetOrgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(msg.get()->data, &jsonError));
    GlobalCenter::getInstance()->signGetOrgRes(jsonDoc);
}



#endif // LOGIN_HANDLE_H
