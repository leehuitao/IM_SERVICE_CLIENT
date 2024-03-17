#ifndef INVITE_WIDGET_H
#define INVITE_WIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QScreen>

#include "Network/packet_define.h"

namespace Ui {
class InviteWidget;
}

class InviteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InviteWidget(QWidget *parent = nullptr);
    ~InviteWidget();

    void init();

    void recvMsgBody(MsgBody body){ m_msyBody = body; }
    void setStatusInfo(const QString& statusInfo = {});
    void setInviteType(int type){ m_type = type;}

signals:
    void signHungUp(MsgBody body, int type);
    void signListen(MsgBody body, int type);

private slots:
    void on_hang_up_toolBtn_clicked();

    void on_listen_toolBtn_clicked();

private:
    Ui::InviteWidget *ui;

    MsgBody m_msyBody;
    int     m_type = 0;
};

#endif // INVITE_WIDGET_H
