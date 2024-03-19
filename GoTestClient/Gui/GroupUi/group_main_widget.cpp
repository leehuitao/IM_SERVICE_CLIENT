#include "group_main_widget.h"
#include "ui_group_main_widget.h"

GroupMainWidget::GroupMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMainWidget)
{
    ui->setupUi(this);
}

GroupMainWidget::~GroupMainWidget()
{
    delete ui;
}

void GroupMainWidget::init()
{

}

void GroupMainWidget::on_create_group_btn_clicked()
{
    CreateGroupDialog diaog;
    diaog.setCurrentOrgDept(m_deptListMap);
    diaog.setCurrentOrgUser(m_userListMap);
    diaog.init();
    if(diaog.exec()){
        auto ret = diaog.getInvitationUsers();
    }
}
