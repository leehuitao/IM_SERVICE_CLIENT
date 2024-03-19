#ifndef GROUP_MAIN_WIDGET_H
#define GROUP_MAIN_WIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "Gui/GroupUi/create_group_dialog.h"
namespace Ui {
class GroupMainWidget;
}

class GroupMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupMainWidget(QWidget *parent = nullptr);
    ~GroupMainWidget();

    void init();

    void updateIcon(int userId);

    void setCurrentOrgDept(QMap<int,QList<DeptStruct>> deptListMap){
         m_deptListMap = deptListMap;
    }

    void setCurrentOrgUser(QMap<int,QList<GlobalUserInfo>> userListMap){
         m_userListMap = userListMap;
    }

private slots:
    void on_create_group_btn_clicked();

private:
    Ui::GroupMainWidget *ui;

    QList<int>                  m_allGroupList;

    QMap<int,int>               m_allUserStatus;

    QMap<int,QList<DeptStruct>> m_deptListMap;

    QMap<int,QList<GlobalUserInfo>> m_userListMap;
};

#endif // GROUP_MAIN_WIDGET_H
