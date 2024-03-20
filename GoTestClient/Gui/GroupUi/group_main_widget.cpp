#include "group_main_widget.h"
#include "ui_group_main_widget.h"
#include "Logic/global_ui_center.h"
GroupMainWidget::GroupMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMainWidget)
{
    ui->setupUi(this);

    connect(ui->group_listWidget, SIGNAL(clicked(QModelIndex)),this, SIGNAL(groupClicked(QModelIndex)));
    connect(ui->group_listWidget, SIGNAL(doubleClicked(QModelIndex)),this, SIGNAL(groupDoubleClicked(QModelIndex)));
}

GroupMainWidget::~GroupMainWidget()
{
    delete ui;
}

void GroupMainWidget::init()
{

}

void GroupMainWidget::clearGroups()
{

}

void GroupMainWidget::updateGroupInfo(GroupBody body)
{
    ui->group_name_label->setText(body.GroupName);
    ui->group_info_label->setText(body.GroupInfo);
    ui->group_size_label->setText(QString::number(AppCache::Instance()->m_groupUsers[body.GroupId].size()));
}

void GroupMainWidget::slotAddNewGroup(GroupBody body)
{
    AppCache::Instance()->m_groupInfos[body.GroupId] = body;
    QListWidgetItem *item = new QListWidgetItem();
    item->setWhatsThis(body.GroupName);
    item->setToolTip(body.GroupId);
    item->setData(Qt::UserRole, body.GroupId);
    ui->group_listWidget->insertItem(0,item);
    auto w = new MsgWidgetItem;

    m_userid2UserChat[body.GroupId] =  w;

    w->setUserInfo(body.GroupName,body.GroupId,m_headImagePath);

//    w->updateMsgInfo(body.Msg,body.SendTime);
    item->setSizeHint(w->minimumSizeHint());
    ui->group_listWidget->setItemWidget(item,w);
    ui->group_listWidget->setCurrentItem(item);
}

void GroupMainWidget::on_create_group_btn_clicked()
{
    CreateGroupDialog diaog;
    diaog.setCurrentOrgDept(m_deptListMap);
    diaog.setCurrentOrgUser(m_userListMap);
    diaog.init();
    if(diaog.exec()){
        auto ret = diaog.getInvitationUsers();
        GroupBody body;
        body.UserId = AppCache::Instance()->m_userId;
        body.GroupName = ret.GroupName;
        body.GroupInfo = ret.GroupInfo;
        QString users;
        for (auto it : ret.users) {
            users.append(QString::number(it));
            users.append(",");
        }
        body.SendUserName = users;
        GlobalUiCenter::getInstance()->createGroup(body);
    }
}

void GroupMainWidget::on_group_listWidget_itemClicked(QListWidgetItem *item)
{
    GlobalUiCenter::getInstance()->groupListWidgetItemClicked(item);
}
