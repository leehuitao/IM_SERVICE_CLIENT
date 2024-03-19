#ifndef GET_USER_GROUPS_HANDLE_H
#define GET_USER_GROUPS_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetUserGroupsHandle
{
public:
    void HandleMessage(NewTcpProtocolPtr msg);

};

void GetUserGroupsHandle::HandleMessage(NewTcpProtocolPtr msg)
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
    QList<GroupStruct> groups;
    // 遍历数组中的每个对象
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject obj = jsonArray[i].toObject();
        GroupStruct g;
        g.groupID = obj["group_id"].toString();
        g.groupName = obj["group_name"].toString();
        g.announcement = obj["announcement"].toString();
        g.createdAt = obj["created_at"].toString();
        g.creatorID = obj["creator_id"].toString();
        groups.append(g);
    }
    GlobalCenter::getInstance()->slotRecvGroups(groups);
}

#endif // GET_USER_GROUPS_HANDLE_H
