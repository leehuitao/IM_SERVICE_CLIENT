#include "create_group_dialog.h"
#include "ui_create_group_dialog.h"
#include "Logic/app_cache.h"
CreateGroupDialog::CreateGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateGroupDialog)
{
    ui->setupUi(this);
}

CreateGroupDialog::~CreateGroupDialog()
{
    delete ui;
}

void CreateGroupDialog::init()
{
    m_selecetedUsersModel = new QStandardItemModel(ui->selected_treeView);
    ui->selected_treeView->header()->hide();
    ui->selected_treeView->setModel(m_selecetedUsersModel);
    ui->selected_treeView->setEditTriggers(QHeaderView::NoEditTriggers);
    ui->selected_treeView->setIconSize(QSize(40,40));
    ui->selected_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->selected_treeView->setContextMenuPolicy(Qt::CustomContextMenu);


    m_orgModel = new QStandardItemModel(ui->org_treeView);
    ui->org_treeView->header()->hide();
    ui->org_treeView->setModel(m_orgModel);
    ui->org_treeView->setEditTriggers(QHeaderView::NoEditTriggers);
    ui->org_treeView->setIconSize(QSize(40,40));
    ui->org_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->org_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    // 连接信号和槽
    connect(m_orgModel, &QStandardItemModel::itemChanged, this, &CreateGroupDialog::handleItemChanged);

    //画组织架构
    for(auto it : m_deptListMap){
        for(auto item : it){
            auto group = new QStandardItem(item.DeptName);
            m_id2UiPointer[item.DeptId] = group;
            if(item.ParentDeptId != 0){
                m_id2UiPointer[item.ParentDeptId]->appendRow(group);
            }else{
                m_orgModel->appendRow(group);
            }
        }
    }
    //画人员
    for(auto it : m_userListMap){
        for(auto user : it){
            if (user.DeptId == 0)
                continue;
            auto item = new QStandardItem();
            auto path = AppCache::Instance()->m_headImagePath[user.UserId];
            QImage image(path);
            image = image.scaled(QSize(64,64));
            // 检查图像是否正确加载
            if (image.isNull()) {
                // 处理错误
                item->setIcon(QIcon(":/resource/offline.png"));
            }else{
                item->setIcon(QIcon(QPixmap::fromImage(image)));
            }
            item->setText(user.UserName);
            item->setSizeHint(QSize(200,50));
            item->setWhatsThis(user.UserName);
            item->setToolTip(QString::number(user.UserId));
            item->setCheckable(1);
            // 存储一个整数标识作为自定义角色数据
            item->setData(user.UserId, Qt::UserRole);
            m_id2UiPointer[user.DeptId]->appendRow(item);
        }
    }
}

void CreateGroupDialog::addSelectedUser(int userId, QString userName)
{
    auto item = new QStandardItem();
    auto path = AppCache::Instance()->m_headImagePath[userId];
    QImage image(path);
    image = image.scaled(QSize(64,64));
    // 检查图像是否正确加载
    if (image.isNull()) {
        // 处理错误
        item->setIcon(QIcon(":/resource/offline.png"));
    }else{
        item->setIcon(QIcon(QPixmap::fromImage(image)));
    }
    item->setText(userName);
    item->setSizeHint(QSize(200,50));
    item->setWhatsThis(userName);
    item->setToolTip(QString::number(userId));

    // 存储一个整数标识作为自定义角色数据
    item->setData(userId, Qt::UserRole);

    // 将条目添加到模型
    m_selecetedUsersModel->appendRow(item);
}

void CreateGroupDialog::on_buttonBox_accepted()
{
    m_groupName = ui->group_name_line->text();
    m_groupInfo = ui->introduction_textEdit->placeholderText();
    m_invitationUsers.clear();
    // 假设model是之前创建的QStandardItemModel的实例
    for (int row = 0; row < m_selecetedUsersModel->rowCount(); ++row) {
        QStandardItem *item = m_selecetedUsersModel->item(row);
        QString text = item->text(); //获取用户名
        int id = item->data(Qt::UserRole).toInt(); // 获取用户ID
        m_invitationUsers.append(id);
    }
}

void CreateGroupDialog::on_buttonBox_rejected()
{

}

void CreateGroupDialog::handleItemChanged(QStandardItem *item)
{
    qDebug() << "Item changed:" << item->text() << ", checked:" << item->checkState();
    auto isChecked =  item->checkState();
    QString text = item->text(); //获取用户名
    int id = item->data(Qt::UserRole).toInt(); // 获取用户ID
    if(isChecked){
        addSelectedUser(id,text);
    }else{
        // 遍历模型中的每个项，查找匹配的userId，并删除它
        for (int row = 0; row < m_selecetedUsersModel->rowCount(); ++row) {
            QModelIndex index = m_selecetedUsersModel->index(row, 0); // 第0列是存储用户名的列

            // 检查每个项的用户数据
            QVariant userData = index.data(Qt::UserRole);
            if (userData.isValid() && userData.toInt() == id) {
                // 如果找到匹配的userId，从模型中删除该项
                m_selecetedUsersModel->removeRow(row);
                break; // 找到并删除了项目，退出循环
            }
        }
    }
}
