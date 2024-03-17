#ifndef MESSAGE_BOX_ITEM_H
#define MESSAGE_BOX_ITEM_H

#include <QWidget>
#include "message_item.h"
namespace Ui {
class MessageBoxItem;
}

class MessageBoxItem : public QWidget
{
    Q_OBJECT

public:
    explicit MessageBoxItem(QWidget *parent = nullptr);
    ~MessageBoxItem();

    void initMsg(const MsgBody &msg , bool updateMsg = 1);

    void initImageMsg(const MsgBody &msg, bool updateMsg = 1);
    /**
        增加文件发送消息
        type = 0  发送文件成功消息
        type = 1  接收文件成功消息
    */
    void initMsg(const QString &filePath,int type = 0);

    void updateMsgReadStatus(int status);

    virtual void resizeEvent(QResizeEvent *event);

    QString getMsgId() const {
        return m_msgId;
    }

    MsgBody getMsgBody() const {
        return m_msgBody;
    }

    QSize getSize(){
        return m_size;
    }

    //设置用户头像
    void setUserAvatar(int userId);
private slots:
    void on_open_file_btn_clicked();

    void on_open_dir_btn_clicked();

    void setDisPlayWid(int index);
private:
    Ui::MessageBoxItem *ui;
    QString m_msgId = "";
    QString m_filePath = "";
    QSize m_size;
    MsgBody m_msgBody;
    QString m_imgPath;
    int m_currentState = -1;
    bool isImageMessage = false;
};

#endif // MESSAGE_BOX_ITEM_H
