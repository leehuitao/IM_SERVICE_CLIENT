#ifndef NEW_CHAT_MAIN_H
#define NEW_CHAT_MAIN_H

#include <QWidget>
#include <QListWidgetItem>
#include "chat_message.h"
#include "../../Network/packet_define.h"
namespace Ui {
class NewChatMain;
}

class NewChatMain : public QWidget
{
    Q_OBJECT

public:
    explicit NewChatMain(QWidget *parent = nullptr);
    ~NewChatMain();
    void clear();
    void addNewMsg(const MsgBody &msg);
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, const MsgBody &message);
    void dealMessageTime(QString curMsgTime);
    void resizeEvent(QResizeEvent *event);
    void parseMsg(const QString &source);
    void updateMsgReadStatus(QString msgId, int status);
private:
    Ui::NewChatMain *ui;
};

#endif // NEW_CHAT_MAIN_H
