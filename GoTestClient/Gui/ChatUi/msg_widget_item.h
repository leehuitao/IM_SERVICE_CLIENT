#ifndef MSG_WIDGET_ITEM_H
#define MSG_WIDGET_ITEM_H

#include <QWidget>
#include <QResizeEvent>
namespace Ui {
class MsgWidgetItem;
}

class MsgWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit MsgWidgetItem(QWidget *parent = nullptr);
    ~MsgWidgetItem();

    void setUserInfo(QString name, int userid ,QString avatarPath);

    void setUserInfo(QString name, QString userid ,QString avatarPath);

    void setUserHead(QString avatarPath);

    void updateMsgInfo(const QString &msg, const QString &time);

    void updateUnread(int number);

    void clearUnread();

    virtual void resizeEvent(QResizeEvent *event);

    int getUserId(){
        return m_userid;
    }
    QString getGroupId(){
        return m_groupid;
    }
    QString getGroupName(){
        return m_groupName;
    }
    QString getUserName(){
        return m_userName;
    }

    QString getAvatar(){
        return m_avatarPath;
    }
private:
    Ui::MsgWidgetItem *ui;

    QString     m_userName;
    int         m_userid;
    QString     m_groupid;
    QString     m_groupName;
    QString     m_avatarPath;
    QString     m_lastMsg;
    QString     m_lastMsgTime;
    QPixmap     m_primitive;//原始图片
};

#endif // MSG_WIDGET_ITEM_H
