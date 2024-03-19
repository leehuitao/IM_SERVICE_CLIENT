#ifndef MESSAGE_INTERFACE_H
#define MESSAGE_INTERFACE_H

#include <QListWidget>
#include <QWidget>
#include "message_box_item.h"
#include "msg_widget_item.h"
#include "Logic/global_center.h"
#include "sql/sqlite.h"
namespace Ui {
class MessageInterface;
}

class MessageInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MessageInterface(QWidget *parent = nullptr);
    ~MessageInterface();

    void addShakeMsg(const QString &msg);

    void addNewMsg(const MsgBody &msg);


    void setMaxiMum();
    /**
        增加文件发送消息
        type = 0  发送文件成功消息
        type = 1  接收文件成功消息
    */
    void addNewFileMsg(const QString &filePath,int type = 0);
    //接收消息
    void slotRecvMsg( MsgBody body);
    //更新消息状态
    void updateMsgReadStatus(QString msgId, int status);
    //更新用户头像
    void updateHead(int userId);

    void clear();

    virtual void showEvent(QShowEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

    void listWidgetItemClicked(QListWidgetItem *item);

    void listWidgetItemClicked(int userId,QString userName);

    void listWidgetItemClicked(QString groupId);

//    void slotUpdateMsgStatus(const MsgBody& body);

    void initDB();

    void setWinId(HWND id);

protected:
private:

    void flashWindow();
private:
    Ui::MessageInterface *ui;
    int currentDisplayUserId = -1;
    //用于刷新界面新消息
    int currentDisplayLastTime = -1;
    //当前显示的消息数量
    int currentDisplayMsgNum = 0;

    Sqlite          m_sql;

    HWND    m_mainWinId;
    QList<MsgBody> m_unreadMsgIdList;
};

#endif // MESSAGE_INTERFACE_H
