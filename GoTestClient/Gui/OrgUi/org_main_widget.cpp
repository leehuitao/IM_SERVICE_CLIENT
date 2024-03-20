#include "org_main_widget.h"
#include "ui_org_main_widget.h"

#include <QPainter>

OrgMainWidget::OrgMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrgMainWidget)
{
    ui->setupUi(this);
    init();
}

OrgMainWidget::~OrgMainWidget()
{
    delete ui;
}

void OrgMainWidget::init()
{
    ui->info_widget->hide();
    m_pModel = new QStandardItemModel(ui->treeView);
    ui->treeView->header()->hide();
    ui->treeView->setModel(m_pModel);
    ui->treeView->setEditTriggers(QHeaderView::NoEditTriggers);
    ui->treeView->setIconSize(QSize(40,40));
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),this, SLOT(slotClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(slotDoubleClicked(QModelIndex)));
}

void OrgMainWidget::slotLoginStatus(int status, QString str)
{
    if(status){
        ui->login_status_lab->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->login_btn->setText(QStringLiteral("退出"));
    }else{
        ui->login_status_lab->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->login_btn->setText("login");
    }
    ui->login_status_str_lab->setText(str);
}

QImage OrgMainWidget::drawLeaveIcon(QString user)
{
    // 加载用户头像和离开的图片
    QImage userIcon(user);
    userIcon = userIcon.scaled(QSize(64,64));
    QImage leaveIcon(":/resource/leave.png");
    leaveIcon = leaveIcon.scaled(QSize(24,24));
    // 创建一个新的 QImage 来容纳合并后的图片
    QImage mergedImage(userIcon.size(), QImage::Format_ARGB32);
    mergedImage.fill(Qt::transparent);

    // 创建一个 QPainter 对象，并将其与合并后的图像关联
    QPainter painter(&mergedImage);
    painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿
    // 将用户头像绘制在合并后的图像上
    painter.drawImage(0, 0, userIcon);

    // 计算离开图片的绘制位置
    int leaveX = 64 - leaveIcon.width();
    int leaveY = 64 - leaveIcon.height();

    // 将离开的图片绘制在右下角
    painter.drawImage(leaveX, leaveY, leaveIcon);

    painter.end();
    return mergedImage;
}

QImage OrgMainWidget::drawIcon(const QImage &img)
{
    return QImage();
}

void OrgMainWidget::slotRecvOnlineUserList(QString userList)
{
    qDebug()<<__FUNCTION__<<userList;
    //    QStringList list = userList.split(",");
    // 解析 JSON 字符串
    QMap<int,int> userStatus;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(userList.toUtf8());
    if (!jsonDocument.isNull() && jsonDocument.isObject()) {
        QJsonObject jsonObject = jsonDocument.object();
        QStringList keys = jsonObject.keys();

        // 遍历解析后的 JSON 对象
        for (const QString &key : keys) {
            QJsonValue value = jsonObject.value(key);
            userStatus[key.toInt()] = value.toInt();
            //qDebug() << "Key:" << key << "Value:" << value.toInt();
        }
    } else {
        qDebug() << "Failed to parse JSON.";
    }
    auto keys = userStatus.keys();
    for(auto it : keys){
        if(it >= 88880000){
            auto status = userStatus[it];
            auto path = AppCache::Instance()->m_headImagePath[it];
            QImage image(path);
            image = image.scaled(QSize(64,64));
            m_allUserStatus[it] = status;
            if(it == AppCache::Instance()->m_userId){
                if (image.isNull()) {
                    if(status == UserLoginStatus){
                        m_userId2UiPointer[it]->setIcon(QIcon(":/resource/self.png"));
                    }else if(status == UserLeaveStatus){
                        QIcon combinedIcon;
                        combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(":/resource/self.png"))); // 调整头像大小
                        m_userId2UiPointer[it]->setIcon(combinedIcon);
                    }
                }else{
                    if(status == UserLoginStatus){
                        m_userId2UiPointer[it]->setIcon(QIcon(QPixmap::fromImage(image)));
                    }else if(status == UserLeaveStatus){
                        QIcon combinedIcon;
                        combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(path))); // 调整头像大小
                        m_userId2UiPointer[it]->setIcon(combinedIcon);
                    }
                }
            }
            else{
                if (image.isNull()) {
                    if(status == UserLoginStatus){
                        m_userId2UiPointer[it]->setIcon(QIcon(":/resource/other.png"));
                    }else if(status == UserLeaveStatus){
                        QIcon combinedIcon;
                        combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(":/resource/other.png"))); // 调整头像大小
                        m_userId2UiPointer[it]->setIcon(combinedIcon);
                    }
                }else{
                    if(status == UserLoginStatus){
                        m_userId2UiPointer[it]->setIcon(QIcon(QPixmap::fromImage(image)));
                    }else if(status == UserLeaveStatus){
                        QIcon combinedIcon;
                        combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(path))); // 调整头像大小
                        m_userId2UiPointer[it]->setIcon(combinedIcon);
                    }
                }
            }

        }
    }

    //更新所有在线人员后   获取未读消息
    SystemBody body;
    body.UserId=AppCache::Instance()->m_userId;

    GlobalCenter::getInstance()->signGetOfflineMsg(body);
}

void OrgMainWidget::slotOnlineUserUpdate(OnlineListBody body)
{
    m_allUserStatus[body.UserId] = body.Status;
    if(body.Status == UserLoginStatus){
        auto path = AppCache::Instance()->m_headImagePath[body.UserId];
        QImage image(path);
        image = image.scaled(QSize(64,64));
        // 检查图像是否正确加载
        if (image.isNull()) {
            if(body.UserId == AppCache::Instance()->m_userId)
                m_userId2UiPointer[body.UserId]->setIcon(QIcon(":/resource/self.png"));
            else{
                if(m_userId2UiPointer[body.UserId])
                    m_userId2UiPointer[body.UserId]->setIcon(QIcon(":/resource/other.png"));
            }
        }else{
            // 转换为灰度
            QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
            m_userId2UiPointer[body.UserId]->setIcon(QIcon(QPixmap::fromImage(image)));
        }

    }else if(body.Status == UserLeaveStatus){
        auto path = AppCache::Instance()->m_headImagePath[body.UserId];
        QImage image(path);
        image = image.scaled(QSize(24,24));
        // 检查图像是否正确加载
        if (image.isNull()) {
            // 创建用户头像和状态图标组合
            QIcon combinedIcon;
            if(body.UserId == AppCache::Instance()->m_userId){
                combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(":/resource/self.png"))); // 调整头像大小
                m_userId2UiPointer[body.UserId]->setIcon(combinedIcon);
            }
            else{
                combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(":/resource/other.png")));
                m_userId2UiPointer[body.UserId]->setIcon(combinedIcon);
            }
        }else{
            QIcon combinedIcon;
            combinedIcon.addPixmap(QPixmap::fromImage(drawLeaveIcon(path) ));
            m_userId2UiPointer[body.UserId]->setIcon(combinedIcon);
        }
    }
    else if(body.Status == UserLogoffStatus){
        auto path = AppCache::Instance()->m_headImagePath[body.UserId];
        QImage image(path);
        image = image.scaled(QSize(64,64));
        // 检查图像是否正确加载
        if (image.isNull()) {
            if(m_userId2UiPointer[body.UserId])
                m_userId2UiPointer[body.UserId]->setIcon(QIcon(":/resource/offline.png"));
        }else{
            // 转换为灰度
            QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
            m_userId2UiPointer[body.UserId]->setIcon(QIcon(QPixmap::fromImage(grayImage)));
        }

    }
}

void OrgMainWidget::drawOrg(QJsonDocument json)
{
    m_pModel->clear();
    m_deptListMap.clear();
    m_id2UiPointer.clear();
    m_userId2UiPointer.clear();
    m_userListMap.clear();
    auto jsonobj = json.object();
    auto keys    = jsonobj.keys();
    for(auto it : keys){
        auto jval = jsonobj.value(it);

        QByteArray details = QByteArray::fromBase64(jval.toString().toLocal8Bit().data());

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(details, &jsonError));
        if(jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "json error!" << jsonError.errorString();
            return;
        }
        auto values = jsonDoc.object();
        QString deptName = QString::fromLocal8Bit( values.value("DeptName").toString().toLocal8Bit());
        int DeptID = values.value("DeptID").toInt();
        int ParentDeptID = values.value("ParentDeptID").toInt();
        int Level = values.value("Level").toInt();
        DeptStruct d;
        d.DeptName = deptName;
        d.DeptId = DeptID;
        d.ParentDeptId = ParentDeptID;
        d.Level = Level;
        m_deptListMap[Level].append(d);
    }
    for(auto it : m_deptListMap){
        for(auto item : it){
            auto group = new QStandardItem(item.DeptName);
            m_id2UiPointer[item.DeptId] = group;
            if(item.ParentDeptId != 0){
                m_id2UiPointer[item.ParentDeptId]->appendRow(group);
            }else{
                m_pModel->appendRow(group);
            }
        }
    }
    ui->treeView->expandAll();
}

void OrgMainWidget::drawUserOrg(QJsonDocument json)
{
    m_allUserList.clear();
    auto jsonobj = json.object();
    auto keys    = jsonobj.keys();
    for(auto it : keys){
        auto jval = jsonobj.value(it);

        QByteArray details = QByteArray::fromBase64(jval.toString().toLocal8Bit().data());

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(details, &jsonError));
        if(jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "json error!" << jsonError.errorString();
            return;
        }
        auto values = jsonDoc.object();
        QString UserName = values.value("UserName").toString();
        int     UserId = values.value("UserId").toInt();
        int     DeptID = values.value("DeptId").toInt();
        GlobalUserInfo u;
        u.UserName = UserName;
        u.UserId = UserId;
        u.DeptId = DeptID;
        m_userListMap[DeptID].append(u);

        LoginBody loginBody;
        loginBody.UserId = values.value("UserId").toInt();
        loginBody.UserLoginName = values.value("UserLoginName").toString();
        loginBody.UserName = values.value("UserName").toString();
        loginBody.PassWord = values.value("PassWord").toString();
        loginBody.Notice = values.value("Notice").toBool();
        loginBody.MacAddress = values.value("MacAddress").toString();
        loginBody.LoginTime = values.value("LoginTime").toString();
        loginBody.LoginStatus = values.value("LoginStatus").toInt();
        loginBody.PhoneNumber = values.value("PhoneNumber").toString();
        loginBody.Email = values.value("Email").toString();
        loginBody.HeadImagePath = values.value("HeadImagePath").toString();
        AppCache::Instance()->m_userInfos[loginBody.UserId] = loginBody;
    }
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
                // 转换为灰度
                QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
                item->setIcon(QIcon(QPixmap::fromImage(grayImage)));
            }
            item->setText(user.UserName);
            item->setSizeHint(QSize(200,50));
            item->setWhatsThis(user.UserName);
            item->setToolTip(QString::number(user.UserId));
            m_allUserList.append(user.UserId);
            m_id2UiPointer[user.DeptId]->appendRow(item);
            m_userId2UiPointer[user.UserId] = item;
        }
    }
    ui->treeView->expandAll();
    GlobalCenter::getInstance()->setAllUserId(m_allUserList);
}

void OrgMainWidget::setUserName(QString name)
{
    ui->username_lab->setText(name);
}

void OrgMainWidget::slotGetOrg(QJsonDocument json)
{
    //获取组织架构后更新人员组织
    SystemBody body;
    body.UserId = AppCache::Instance()->m_userId;
    body.SystemCMD      = "0";
    drawOrg(json);
    //    mainWindowNew.slotGetOrg(json);
    GlobalCenter::getInstance()->signGetOrg(body,GetUserOrg,0);
}

void OrgMainWidget::slotGetUserOrg(QJsonDocument json)
{
    //    qDebug()<<json;
    //获取人员组织后更新在线人员
    SystemBody body;
    body.UserId  = AppCache::Instance()->m_userId;
    body.SystemCMD      = "0";
    drawUserOrg(json);
    //    mainWindowNew.slotGetUserOrg(json);
    GlobalCenter::getInstance()->signGetOrg(body,GetOnlineUser,0);
}

void OrgMainWidget::updateIcon(int userId)
{
    auto item = m_userId2UiPointer[userId];
    if(item){
        auto path = AppCache::Instance()->m_headImagePath[userId];
        QImage image(path);
        image = image.scaled(QSize(64,64));
        // 检查图像是否正确加载
        if (image.isNull()) {
            // 处理错误
            if(m_allUserStatus[userId] == UserLoginStatus){
                item->setIcon(QIcon(":/resource/other.png"));
            }else if(m_allUserStatus[userId] == UserLeaveStatus){
                QIcon combinedIcon;
                combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(":/resource/other.png"))); // 调整头像大小
                item->setIcon(combinedIcon);
            }else{
                item->setIcon(QIcon(":/resource/offline.png"));
            }


        }else{
            if(m_allUserStatus[userId] == UserLoginStatus){
                // 转换为灰度
                item->setIcon(QIcon(QPixmap::fromImage(image)));
            }else if(m_allUserStatus[userId] == UserLeaveStatus){
                QIcon combinedIcon;
                combinedIcon.addPixmap(QPixmap::fromImage( drawLeaveIcon(path))); // 调整头像大小
                item->setIcon(combinedIcon);
            }else{
                // 转换为灰度
                QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
                item->setIcon(QIcon(QPixmap::fromImage(grayImage)));
            }

        }
//        auto path = AppCache::Instance()->m_headImagePath[userId];
//        item->setIcon(QPixmap(path.isEmpty() ?":/resource/other.png":path));
    }
}


void OrgMainWidget::slotClicked(const QModelIndex &index)
{
    clicked(index);
    auto currentChoiseUser = index.data(Qt::WhatsThisRole).toString();
    auto currentChoiseUserId = index.data(Qt::ToolTipRole).toInt();
    //显示一下人员信息
    ui->widget->setStyleSheet("");
    ui->info_widget->show();
    auto path = AppCache::Instance()->m_headImagePath[currentChoiseUserId];
    ui->head_image_label->setPixmap(QPixmap(path.isEmpty() ?":/resource/other.png":path));
    ui->name_label->setText(currentChoiseUser);
    ui->phone_number_label->setText(QString::number(currentChoiseUserId));
}

void OrgMainWidget::slotDoubleClicked(const QModelIndex &index)
{
    doubleClicked(index);
}

void OrgMainWidget::on_send_btn_clicked()
{
    auto index = ui->treeView->currentIndex();
    doubleClicked(index);
}
#include "Logic/global_center.h"
void OrgMainWidget::on_login_btn_clicked()
{
    LoginBody body;
    body.UserId     =AppCache::Instance()->m_userId;
    body.UserLoginName   = AppCache::Instance()->m_userLoginName;
    body.UserName   = AppCache::Instance()->m_userName;
    GlobalCenter::getInstance()->signLogout(body);
}
