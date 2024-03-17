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

    void addNewUser(const MsgBody & body);

    void addNewUser(int userId,QString userName);

    void updateMsgInfo(int userId,const QString& msg,const QString& time);

    void updateMsgItemSort(int userId);

    void setCurrentUser(int userId);

    void setUserHeadImage(int userId);
public slots:
    void slotUpdataUnread(int id,int number);
signals:
    void signCurrentUserListUpdate(QList<int>);
private:
    //用于存储聊天人员的左侧按钮
    QMap<int,MsgWidgetItem*>    m_userid2UserChat;

    QString m_headImagePath = ":/resource/other.png";
};



#endif // HISTORICALUSERLIST_H
