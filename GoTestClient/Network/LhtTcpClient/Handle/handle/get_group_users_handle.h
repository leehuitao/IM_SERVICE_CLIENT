#ifndef GET_GROUP_USERS_HANDLE_H
#define GET_GROUP_USERS_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetGroupUsersHandle
{
public:
    void HandleMessage(NewTcpProtocolPtr msg);

};

void GetGroupUsersHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    auto body = PacketProcess::parseGroupBodyPack(msg.get()->data);
    // 将QString转换为QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(body.Msg.toUtf8());

    // 检查文档是否包含一个数组
    if(!doc.isArray()) {
        qDebug() << "JSON document does not contain an array";
        return;
    }

    // 获取JSON数组
    QJsonArray jsonArray = doc.array();
    QList<GroupUsersStruct> groups;
    // 遍历数组中的每个对象
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject obj = jsonArray[i].toObject();
        GroupUsersStruct g;
        g.Id =          obj["id"].toInt();
        g.GroupID =     obj["group_id"].toString();
        g.UserId =      obj["user_id"].toInt();
        g.JoinAt =      obj["join_at"].toString();
        g.UserLevel =   obj["user_level"].toInt();
        groups.append(g);
    }
    GlobalCenter::getInstance()->slotRecvGroupUsers(groups);
}

#endif // GET_GROUP_USERS_HANDLE_H
