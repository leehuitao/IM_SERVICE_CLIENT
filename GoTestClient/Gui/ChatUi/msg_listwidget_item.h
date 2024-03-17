#ifndef MSGLISTWIDGETITEM_H
#define MSGLISTWIDGETITEM_H

#include <QObject>
#include <QListWidget>
#include <QLabel>
class MsgListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    MsgListWidgetItem(QWidget * parent = nullptr);

    void init(QString name, int userid ,QString avatarPath);

private:

    QString     m_userName;
    int         m_userid;
    QString     m_avatarPath;
    QString     m_lastMsg;
    QString     m_lastMsgTime;

    QLabel      *m_avatarLab;
    QLabel      *m_userNameLab;
    QLabel      *m_lastMsgLab;
    QLabel      *m_lastMsgTimeLab;


};

#endif // MSGLISTWIDGETITEM_H
