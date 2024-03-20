#ifndef HISTORICALUSERLIST_H
#define HISTORICALUSERLIST_H

#include <QObject>
#include <QListWidget>
#include "Logic/global_center.h"
#include "msg_widget_item.h"
class HistoricalUserList : public QListWidget
{
    Q_OBJECT
public:
    HistoricalUserList(QWidget *parent = nullptr);

    bool checkUserIsExists(int userId);

    bool checkUserIsExists(QString groupId);

    void addNewUser(const MsgBody & body);

    void addNewUser(int userId,QString userName);

    void addNewUser(GroupBody body);

    void addNewUser(QString groupId,QString userName);

    void updateMsgInfo(int userId,const QString& msg,const QString& time);

    void updateGroupMsgInfo(QString groupId,const QString& msg,const QString& time);

    void updateMsgItemSort(int userId);

    void updateMsgItemSort(QString groupId);

    void setCurrentUser(int userId);

    void setCurrentGroup(QString groupId);

    void setUserHeadImage(int userId);
public slots:
    void slotUpdataUnread(int id,int number);

    void slotUpdataUnread(QString id,int number);
signals:
    void signCurrentUserListUpdate(QList<int>);
private:
    //用于存储聊天人员的左侧按钮
    QMap<int,MsgWidgetItem*>    m_userid2UserChat;
    //用于存储聊天人员的左侧按钮
    QMap<QString,MsgWidgetItem*>    m_groupid2UserChat;

    QString m_headImagePath = ":/resource/other.png";
};



#endif // HISTORICALUSERLIST_H
