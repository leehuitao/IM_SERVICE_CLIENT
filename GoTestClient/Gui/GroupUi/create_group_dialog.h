#ifndef CREATE_GROUP_DIALOG_H
#define CREATE_GROUP_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include "Network/packet_define.h"
#include "Logic/global_center.h"
namespace Ui {
class CreateGroupDialog;
}

class CreateGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGroupDialog(QWidget *parent = nullptr);
    ~CreateGroupDialog();
    void init();
    //需要绘制头像，用户名，将用户ID添加到属性表里
    void addSelectedUser(int userId,QString userName);

    GroupInvitationStruct getInvitationUsers(){
        GroupInvitationStruct s;
        s.GroupName = m_groupName;
        s.GroupInfo = m_groupInfo;
        s.users = m_invitationUsers;
        return s;
    }

    void setCurrentOrgDept(QMap<int,QList<DeptStruct>> deptListMap){
         m_deptListMap = deptListMap;
    }

    void setCurrentOrgUser(QMap<int,QList<GlobalUserInfo>> userListMap){
         m_userListMap = userListMap;
    }

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void handleItemChanged(QStandardItem *item);
private:
    Ui::CreateGroupDialog *ui;

    QString                         m_groupName;

    QString                         m_groupInfo;

    QList<int>                      m_invitationUsers;

    QStandardItemModel *            m_selecetedUsersModel;

    QStandardItemModel *            m_orgModel;

    QMap<int,QList<DeptStruct>>     m_deptListMap;

    QMap<int,QStandardItem*>    m_id2UiPointer;

    QMap<int,QList<GlobalUserInfo>> m_userListMap;
};

#endif // CREATE_GROUP_DIALOG_H
