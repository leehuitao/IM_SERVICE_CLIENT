#ifndef LOGIN_HANDLE_H
#define LOGIN_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class LoginHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void LoginHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }

    LoginBody body;
    body = PacketProcess::parseLoginPack(msg.get()->data);
    AppCache::Instance()->m_userId = body.UserId;
    GlobalCenter::getInstance()->slotLoginStatus(body.LoginStatus,body.LoginStatus ? "登录成功":"登录失败");
    GlobalCenter::getInstance()->signLoginBody(body);
}



#endif // LOGIN_HANDLE_H
