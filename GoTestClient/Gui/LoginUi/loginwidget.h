#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>
#include <QDebug>
#include <QSystemTrayIcon>
#include "Logic/app_cache.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "login_set_dialog.h"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    //透明度动画
    void transparenceAnimation();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

    void on_min_btn_clicked();

    void on_close_btn_clicked();

    void toggleWindow();

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void initSysMenu();

    void initView();

    void on_display_pwd_btn_clicked();

    void on_set_btn_clicked();

    void slotLoginStatus(int status,QString str = "");

    void slotTcpConnected();

    void slotLogout();
private:
    bool checkUser(QString account,QString password);
    void setShadowEffect(QWidget * w);
    MainWindow w;
    Ui::LoginWidget *ui;
    QTimer *timer;
    QLabel * label;
    QPropertyAnimation *animation;
    QGraphicsOpacityEffect *goe;
    QGraphicsColorizeEffect * gce;
    float m_opacityVal = 1.0f;
    QPoint lastMousePos;
    QPoint lastWindowPos;
    bool dragging = false;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QTimer              m_tcpTimer;
    LoginSetDialog      m_loginSetDialog;
};

#endif // LOGINWIDGET_H
