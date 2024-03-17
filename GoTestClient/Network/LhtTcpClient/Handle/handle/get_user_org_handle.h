#ifndef GET_USER_ORG_HANDLE_H
#define GET_USER_ORG_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetUserOrgHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetUserOrgHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(msg.get()->data, &jsonError));
    GlobalCenter::getInstance()->slotGetUserOrg(jsonDoc);
}



#endif // LOGIN_HANDLE_H
