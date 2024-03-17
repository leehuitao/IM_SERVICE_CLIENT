#ifndef ORG_MAIN_WIDGET_H
#define ORG_MAIN_WIDGET_H

#include <QStandardItemModel>
#include <QWidget>
#include "Network/packet_define.h"
#include "Logic/global_center.h"
namespace Ui {
class OrgMainWidget;
}

class OrgMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrgMainWidget(QWidget *parent = nullptr);
    ~OrgMainWidget();

    void init();
    void slotLoginStatus(int status,QString str);

    void slotRecvOnlineUserList(QString userList);

    void slotOnlineUserUpdate(OnlineListBody body);

    void drawOrg(QJsonDocument);

    void drawUserOrg(QJsonDocument);

    void setUserName(QString name);

    void slotGetOrg(QJsonDocument json);

    void slotGetUserOrg(QJsonDocument json);

    void updateIcon(int userId);

    QList<int> getAllUserIdList(){
        return m_userId2UiPointer.keys();
    }
protected slots:
    void slotClicked(const QModelIndex &index);

    void slotDoubleClicked(const QModelIndex &index);
signals:
    void clicked(const QModelIndex &index);

    void doubleClicked(const QModelIndex &index);

    void signLogout();
private slots:
    void on_send_btn_clicked();

    void on_login_btn_clicked();

private:
    QImage drawLeaveIcon(QString user);
    QImage drawIcon(const QImage& img);
private:
    Ui::OrgMainWidget *ui;

    QList<int>                  m_allUserList;

    QMap<int,int>               m_allUserStatus;

    QMap<int,QStandardItem*>    m_userId2UiPointer;

    QStandardItemModel*         m_pModel;

    QMap<int,QList<DeptStruct>> m_deptListMap;

    QMap<int,QStandardItem*>    m_id2UiPointer;

    QMap<int,QList<GlobalUserInfo>> m_userListMap;
};

#endif // ORG_MAIN_WIDGET_H
