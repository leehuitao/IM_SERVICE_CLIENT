#include "global_ui_center.h"
#include "Gui/ChatUi/screenwidget.h"
#include "global_center.h"

#include <QPropertyAnimation>
#include <QUuid>
#include <QFileDialog>
GlobalUiCenter* GlobalUiCenter::_instance = new  GlobalUiCenter;

void GlobalUiCenter::initUi()
{
    m_sql.initDB();
    m_sql.createDafultTable();

    m_setDialog = new SettingDialog;

    m_sendTextEdit->installEventFilter(m_mainWindow);//设置完后自动调用其eventFilter函数

    m_headWidget->setHeadImage(":/resource/login/boy_header.png");

    m_inviteWidget = new InviteWidget;
    connect(m_inviteWidget,&InviteWidget::signListen,[=](MsgBody body, int type){
        if(type == 0)//语音
        {
            slotStartAudio(body);
        }
        else        //视频
        {
            slotStartVideo(body);
        }
    });

    connect(this,SIGNAL(signUpdataUnread(int,int)) ,m_historicalUserList,SLOT(slotUpdataUnread(int,int)));
    connect(this,SIGNAL(signUpdataUnread(QString,int)) ,m_historicalUserList,SLOT(slotUpdataUnread(QString,int)));
    connect(m_historicalUserList,&HistoricalUserList::signCurrentUserListUpdate,this,&GlobalUiCenter::slotCurrentUserListUpdate);

    connect(m_orgMainWidget, SIGNAL(clicked(QModelIndex)),this, SLOT(clicked(QModelIndex)));
    connect(m_orgMainWidget, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(doubleClicked(QModelIndex)));


    connect(m_groupWidget, SIGNAL(groupClicked(QModelIndex)),this, SLOT(groupClicked(QModelIndex)));
    connect(m_groupWidget, SIGNAL(groupDoubleClicked(QModelIndex)),this, SLOT(groupDoubleClicked(QModelIndex)));

    connect(ScreenWidget::Instance(),&ScreenWidget::signHasImag,this,&GlobalUiCenter::slotHasImag);

    connect(GlobalCenter::getInstance() ,&GlobalCenter::signLoginStatus,       this,&GlobalUiCenter::slotLoginStatus               ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signLoginBody,         this,&GlobalUiCenter::slotLoginBody                 ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signRecvMsg,           this,&GlobalUiCenter::slotRecvMsg                   ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signRecvFileProgress,  this,&GlobalUiCenter::slotRecvFileProgress          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signOnlineUserList,    this,&GlobalUiCenter::slotRecvOnlineUserList        ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signOnlineUserUpdate,  this,&GlobalUiCenter::slotOnlineUserUpdate          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signSendFileProgress,  this,&GlobalUiCenter::slotSendFileProgress          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signRecvFileCompelte,  this,&GlobalUiCenter::slotRecvFileCompelte          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signSendFileCompelte,  this,&GlobalUiCenter::slotSendFileCompelte          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signGetOrgRes,         this,&GlobalUiCenter::slotGetOrg                    ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signGetUserOrg,        this,&GlobalUiCenter::slotGetUserOrg                ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signRecvMsgNotify,     this,&GlobalUiCenter::slotRecvMsgNotify          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signMsgHadBeenRead,    this,&GlobalUiCenter::slotUpdateMsgStatus          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signGetOfflineNotify,  this,&GlobalUiCenter::slotGetOfflineNotify          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signUpdateUserHeadImage,this,&GlobalUiCenter::slotUpdateUserHeadImage          ,Qt::QueuedConnection);

    connect(GlobalCenter::getInstance() ,&GlobalCenter::signStartAudio         ,this,&GlobalUiCenter::slotInviteAudio         ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signAccptAudio         ,this,&GlobalUiCenter::slotAccptAudio          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signStopAudio         , this,&GlobalUiCenter::slotCloseVoice          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signStartVideo         ,this,&GlobalUiCenter::slotInviteVideo         ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signAccptVideo         ,this,&GlobalUiCenter::slotAccptVideo          ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signStopVideo         , this,&GlobalUiCenter::slotCloseVideo         ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signImageReadReady,      this,&GlobalUiCenter::slotImageReadReady,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signRemoteImageReadReady,this,&GlobalUiCenter::slotRemoteImageReadReady,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signCloseFile,this,&GlobalUiCenter::slotCloseFile,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signCreateGroup,this,&GlobalUiCenter::slotCreateGroup,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signRecvGroups,this,&GlobalUiCenter::slotRecvGroups,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signRecvGroupUsers,this,&GlobalUiCenter::slotRecvGroupUsers,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signNewGroupMsg,this,&GlobalUiCenter::slotNewGroupMsg,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance(),&GlobalCenter::signGetGroupOfflineMsg,this,&GlobalUiCenter::slotRecvGroupMsgNotify,Qt::QueuedConnection);



    GlobalCenter::getInstance()->initConnection();

}

GlobalUiCenter::GlobalUiCenter(QObject *parent) : QObject(parent)
{

}

void GlobalUiCenter::on_set_btn_clicked()
{
    m_setDialog->show();
}

void GlobalUiCenter::clicked(const QModelIndex &index)
{
    //    auto currentChoiseUser = index.data(Qt::WhatsThisRole).toString();
    //    auto currentChoiseUserId = index.data(Qt::ToolTipRole).toInt();
}

void GlobalUiCenter::doubleClicked(const QModelIndex &index)
{
    m_currentChatType = 1;
    auto currentChoiseUser = index.data(Qt::WhatsThisRole).toString();
    auto currentChoiseUserId = index.data(Qt::ToolTipRole).toInt();
    if(currentChoiseUserId == 0)
        return;
    AppCache::Instance()->m_msgSize = 0;
    bool ret = m_historicalUserList->checkUserIsExists(currentChoiseUserId);//判断是否已经存在了
    if(!ret){
        m_historicalUserList->addNewUser(currentChoiseUserId,currentChoiseUser);
    }else{
        m_historicalUserList->setCurrentUser(currentChoiseUserId);
    }
    msgListWidgetItemClicked(currentChoiseUserId,currentChoiseUser);

    GlobalCenter::getInstance()->setCurrentUser(currentChoiseUserId,currentChoiseUser);

    m_stackedWidget->setCurrentIndex(1);

    m_historicalUserList->slotUpdataUnread(currentChoiseUserId,0);
}

void GlobalUiCenter::on_msg_listWidget_itemClicked(QListWidgetItem *item)
{
    int before = GlobalCenter::getInstance()->currentUserId();
    if(before > 0)
        m_fileWidgets[before]->hide();
    QListWidget *widget = item->listWidget();
    if (widget) {
        // 获取与QListWidgetItem关联的控件
        QWidget *widgetItem = widget->itemWidget(item);
        if (widgetItem) {
            // 检查控件类型并执行相应操作
            if (qobject_cast<MsgWidgetItem*>(widgetItem)) {
                MsgWidgetItem *wid = qobject_cast<MsgWidgetItem*>(widgetItem);
                auto currentChoiseUser = wid->getUserName();
                auto currentChoiseUserId = wid->getUserId();
                if(!currentChoiseUser.isEmpty()){
                    m_currentChatType = 1;
                    wid->clearUnread();
                    wid->updateUnread(0);
                    msgListWidgetItemClicked(currentChoiseUserId,currentChoiseUser);
                    int after = GlobalCenter::getInstance()->currentUserId();
                    if(m_fileWidgets[after]->getRemainderFileCount()>0)
                    {
                        m_fileWidgets[after]->show();
                    }
                }else{
                    m_currentChatType = 2;
                    auto currentChoiseGroupId = wid->getGroupId();
                    auto currentChoiseGroupName = wid->getGroupName();
                    wid->clearUnread();
                    wid->updateUnread(0);
                    groupListWidgetItemClicked(currentChoiseGroupId);
                }

            }
        }
    }

    //    m_currentMessageInterface->listWidgetItemClicked(item);

}

void GlobalUiCenter::on_file_btn_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(m_mainWindow);
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("File(*.*)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }
    if(fileNames.size()>0)
    {
        AppCache::Instance()->m_currentFilePath = fileNames.at(0);
        m_fileWidgets[GlobalCenter::getInstance()->currentUserId()]->show();
        m_fileWidgets[GlobalCenter::getInstance()->currentUserId()]->addFileList(fileNames.at(0),createFileMd5(fileNames.at(0)));
        m_fileGridLayout->addWidget(m_fileWidgets[GlobalCenter::getInstance()->currentUserId()], 0, 2);
    }
}

void GlobalUiCenter::slotCloseFile(FileBody body)
{
    int uid = GlobalCenter::getInstance()->currentUserId();
    if(body.SendUserId != 0){
        uid = AppCache::Instance()->m_userId == body.SendUserId ?body.RecvUserId : body.SendUserId;
    }
    m_fileWidgets[uid]->closeFile(body.FileMD5);

}

void GlobalUiCenter::on_sendmsg_btn_clicked()
{
    auto msg = m_sendTextEdit->getMsg();
    QString imgStart = ImageHeader;
    QString imgEnd = ImageEnd;
    QStringList textList ;
    QList<bool > isImageMsg;
    //将图片与文字分开
    auto srcList = msg.split(imgStart);
    for(auto iter :srcList){
        if(iter.contains(ImageEnd)){
            if(iter.split(ImageEnd).size() > 1 ){
                textList.append(iter.split(ImageEnd).at(0));
                isImageMsg.append(1);
                if(!iter.split(ImageEnd).at(1).isEmpty()){
                    textList.append(iter.split(ImageEnd).at(1));
                    isImageMsg.append(0);
                }
            }
        }else{
            textList.append(iter);
            isImageMsg.append(0);
        }
    }
    for(auto it : textList){
        auto iImg = isImageMsg.takeFirst();
        auto fMsg = it;
        if(iImg){
            fMsg = ImageHeader+it+ImageEnd;
        }
        if(it == " " || it.isEmpty())
            continue;
        if(m_currentChatType == 1){
            auto body = GlobalCenter::getInstance()->sendMsg(fMsg);
            m_historicalUserList->updateMsgItemSort(GlobalCenter::getInstance()->currentUserId());
            m_sql.insertHistoryMsg(body);
            m_historicalUserList->updateMsgInfo(body.DstUserId,body.Msg,body.SendTime);
            if(m_currentMessageInterface){
                m_currentMessageInterface->addNewMsg(body);
            }
        }else if(m_currentChatType == 2){
            auto body = GlobalCenter::getInstance()->sendGroupMsg(fMsg,0,SendGroupMsg);
            m_historicalUserList->updateMsgItemSort(GlobalCenter::getInstance()->currentGroupId());
            m_sql.insertGroupHistoryMsg(body);
            m_historicalUserList->updateGroupMsgInfo(body.GroupId,body.Msg,body.SendTime);
            if(m_currentMessageInterface){
                m_currentMessageInterface->addNewGroupMsg(body);
            }
        }

        //        m_messageInterface->addNewMsg(body);

    }
    m_sendTextEdit->clear();
}

void GlobalUiCenter::slotCurrentUserListUpdate(QList<int> list)
{
    auto keys = m_fileWidgets.keys();
    for(auto it:list)
    {
        if(!keys.contains(it))
        {
            FileWidget * fileWidget = new FileWidget();
            connect(fileWidget,&FileWidget::signFileCmd,GlobalCenter::getInstance(),&GlobalCenter::slotFileCmd);
            m_fileWidgets[it] = fileWidget;
        }
    }
}

void GlobalUiCenter::slotHasImag()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage())
    {
        //将图片数据转为QImage
        static int count = 0;
        QImage img = qvariant_cast<QImage>(mimeData->imageData());
        QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(count++);
        img.save(fileName);
        QTextDocumentFragment fragment;
        fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(fileName));
        m_sendTextEdit->textCursor().insertFragment(fragment);
        m_sendTextEdit->setVisible(true);
        m_sendTextEdit->setToolTip("hasImag");
    }
}

void GlobalUiCenter::createGroup(GroupBody b)
{
    GlobalCenter::getInstance()->createGroup(b);
}

void GlobalUiCenter::slotCreateGroup(GroupBody b)
{
    m_groupWidget->slotAddNewGroup(b);
}

void GlobalUiCenter::groupListWidgetItemClicked(QListWidgetItem *item)
{

    //文件相关的先不做，先做消息
    //    m_currentMessageInterface->listWidgetItemClicked(item);
    //    int after = GlobalCenter::getInstance()->currentUserId();
    //    if(m_fileWidgets[after]->getRemainderFileCount()>0)
    //    {
    //        m_fileWidgets[after]->show();
    //    }
}

void GlobalUiCenter::groupListWidgetItemClicked(QString groupId)
{
    m_currentChatType = 2;
    // 隐藏所有的聊天界面
    for (int i = m_messageInterface->count() - 1; i >= 0; --i) {
        auto item = m_messageInterface->itemAt(i);
        if (item->widget()) { // 确保这是一个小部件
            auto widget = item->widget();
            if (qobject_cast<MessageInterface *>(widget)) {
                widget->hide();
            }
        }
    }

    if(m_groupInterfaceMap.find(groupId) == m_groupInterfaceMap.end()){//只在第一次点击的时候 载入一次数据库数据
        m_currentMessageInterface  = new MessageInterface();
        m_groupInterfaceMap[groupId] = m_currentMessageInterface;
        m_messageInterface->addWidget(m_currentMessageInterface);
        m_groupInterfaceMap[groupId]->listWidgetItemClicked(groupId);
        m_currentMessageInterface->show();


    }else{
        GlobalCenter::getInstance()->setCurrentGroupId(groupId);
        m_currentMessageInterface = m_groupInterfaceMap[groupId];
        m_currentMessageInterface->show();
        //        m_messageInterface->addWidget(m_currentMessageInterface);
    }

}

void GlobalUiCenter::groupDoubleClicked(const QModelIndex &index)
{
    m_currentChatType = 2;
    auto currentChoiseGroup = index.data(Qt::WhatsThisRole).toString();
    auto currentChoiseGroupId = index.data(Qt::ToolTipRole).toString();
    if(currentChoiseGroupId.isEmpty())
        return;
    AppCache::Instance()->m_msgSize = 0;
    bool ret = m_historicalUserList->checkUserIsExists(currentChoiseGroupId);//判断是否已经存在了
    if(!ret){
        m_historicalUserList->addNewUser(currentChoiseGroupId,currentChoiseGroup);
    }else{
        m_historicalUserList->setCurrentGroup(currentChoiseGroupId);
    }
    groupListWidgetItemClicked(currentChoiseGroupId);

    GlobalCenter::getInstance()->setCurrentGroupId(currentChoiseGroupId);

    m_stackedWidget->setCurrentIndex(1);

    m_historicalUserList->slotUpdataUnread(currentChoiseGroupId,0);
}

void GlobalUiCenter::groupClicked(const QModelIndex &index)
{
    auto currentChoiseGroupId = index.data(Qt::ToolTipRole).toString();
    m_groupWidget->updateGroupInfo(AppCache::Instance()->m_groupInfos[currentChoiseGroupId]);
}
//流程
//1.登录
//2. slotLoginStatus 中获取组织架构
//3. slotGetOrg 中获取人员列表
//4. 获取在线人员列表
//5. slotRecvOnlineUserList中 获取单人的离线通知消息
//4. slotGetUserOrg中  获取群组
//5. slotRecvGroups中  获取群组人员信息
//6. slotRecvGroupUsers中  绘制历史消息，获取离线群组消息
void GlobalUiCenter::slotRecvGroups(QList<GroupStruct> g)
{
    GroupBody body;
    for(auto it : g){
        body.GroupId = it.groupID;
        body.GroupName = it.groupName;
        body.GroupInfo = it.announcement;
        m_groupWidget->slotAddNewGroup(body);
        //继续获取所有群组的人员列表
        GlobalCenter::getInstance()->slotGetGroupUsers(it.groupID);
    }

    drawHistoryLastMsg();
    //获取离线群组消息
    GlobalCenter::getInstance()->slotGetGroupOfflineMsg();
}

void GlobalUiCenter::slotRecvGroupUsers(QList<GroupUsersStruct> g)
{
    if(g.size() > 0){
        AppCache::Instance()->m_groupUsers[g.first().GroupID].clear();
        AppCache::Instance()->m_groupUsers[g.first().GroupID].append(g);
    }
}

void GlobalUiCenter::slotNewGroupMsg(GroupBody body)
{
    if(m_historicalUserList->checkUserIsExists(body.GroupId)){
        m_historicalUserList->updateGroupMsgInfo(body.GroupId,body.Msg,body.SendTime);
    }else{//如果不存在则新建
        m_historicalUserList->addNewUser(body.GroupId,body.GroupName);
//        groupListWidgetItemClicked(body.GroupId);
        m_historicalUserList->updateGroupMsgInfo(body.GroupId,body.Msg,body.SendTime);
        //            on_msg_listWidget_itemClicked(item);
    }

    m_sql.insertGroupHistoryMsg(body);
    if(m_groupInterfaceMap.find(body.GroupId) != m_groupInterfaceMap.end()){
        auto msgWid = m_groupInterfaceMap[body.GroupId];
        if(msgWid){
            msgWid->addNewGroupMsg(body);
        }
    }
    //更新列表 提示未读
    int unRead = m_sql.getUnreadNumber(body.GroupId,AppCache::Instance()->m_userId);
    signUpdataUnread(body.GroupId, unRead);


}

void GlobalUiCenter::slotRecvGroupMsgNotify(GroupBody body)
{
    auto list = body.MsgId.split("|");
    for(auto it : list){
        if(!it.isEmpty())
            getGroupMsg(it);
    }
}

void GlobalUiCenter::getGroupMsg(QString msgid)
{
    GlobalCenter::getInstance()->slotGetGroupMsg(msgid);
}

void GlobalUiCenter::slotLoginStatus(int status, QString str)
{
    m_orgMainWidget->slotLoginStatus(status,str);
}

void GlobalUiCenter::slotLoginBody(LoginBody body)
{
    m_orgMainWidget->setUserName(body.UserName);
    m_setDialog->init(body);
    AppCache::Instance()->m_userName = body.UserName;
    AppCache::Instance()->m_userLoginName = body.UserLoginName;
}

void GlobalUiCenter::slotRecvMsg(MsgBody body)
{
    if(m_messageInterfaceMap.find(body.SendUserId) != m_messageInterfaceMap.end()){
        auto msgWid = m_messageInterfaceMap[body.SendUserId];
        if(msgWid){
            msgWid->slotRecvMsg(body);
        }
    }

    //    m_messageInterface->slotRecvMsg(body);
    if(body.DstUserId == AppCache::Instance()->m_userId){//接收到消息了

        if(m_historicalUserList->checkUserIsExists(body.SendUserId)){
            if(body.MsgType == 4){
                shakeWindow();
                m_historicalUserList->updateMsgInfo(body.SendUserId,ShakeRecv,body.SendTime);
            }else{
                m_historicalUserList->updateMsgInfo(body.SendUserId,body.Msg,body.SendTime);
            }


        }else{//如果不存在则新建
            m_historicalUserList->addNewUser(body);
            msgListWidgetItemClicked(body.SendUserId,body.SendUserName);
            if(body.MsgType == 4){
                shakeWindow();
                m_historicalUserList->updateMsgInfo(body.SendUserId,ShakeSend,body.SendTime);
            }else{
                m_historicalUserList->updateMsgInfo(body.SendUserId,body.Msg,body.SendTime);
            }
            //            on_msg_listWidget_itemClicked(item);
        }
        //更新列表 提示未读
        int unRead = m_sql.getUnreadNumber(body.SendUserId,AppCache::Instance()->m_userId);
        signUpdataUnread(body.SendUserId, unRead);

    }
    qDebug()<<__FUNCTION__<<body.Msg;
}

void GlobalUiCenter::slotRecvFileProgress(FileBody bodyf)
{
    if(bodyf.TotalSize == bodyf.FileSeek + bodyf.CurrentSize)
    {
        if(m_messageInterfaceMap.find(bodyf.SendUserId) != m_messageInterfaceMap.end()){
            auto msgWid = m_messageInterfaceMap[bodyf.SendUserId];
            if(msgWid){
                msgWid->addNewFileMsg(bodyf.FileName,1);
            }
        }
        //        m_messageInterface->addNewFileMsg(bodyf.FileName,1);
        MsgBody body;
        // 生成一个随机的 UUID
        QUuid uuid = QUuid::createUuid();
        // 将 UUID 转换为字符串表示形式
        QString uuidString = uuid.toString();
        body.MsgId = uuidString;
        body.Msg = bodyf.FileName;
        body.UserId = AppCache::Instance()->m_userId;
        body.SendUserId =  GlobalCenter::getInstance()->currentUserId();
        body.SendUserName = GlobalCenter::getInstance()->currentUserName();
        body.DstUserId = AppCache::Instance()->m_userId;
        body.DstUserName = AppCache::Instance()->m_userName;
        body.SendTime = GlobalCenter::getInstance()->getCurrentTimeSeconds();
        body.MsgType = 1;
        body.MsgStatus = 2;
        m_sql.insertHistoryMsg(body);
    }
    if(m_fileWidgets[bodyf.SendUserId] == nullptr){
        return;
    }
    if(bodyf.FileSeek != 0 || bodyf.FileSeek == 8192)
    {
        m_fileWidgets[bodyf.SendUserId]->setProgress(bodyf.TotalSize,bodyf.FileSeek + bodyf.CurrentSize,1,bodyf.FileName);
        return;
    }

    //进度值为0，说明是新的接收文件，在界面中创建出来
    m_fileWidgets[bodyf.SendUserId]->show();
    m_fileWidgets[bodyf.SendUserId]->addFileList(bodyf.FileName,bodyf.FileMD5, 1);
    m_fileGridLayout->addWidget(m_fileWidgets[bodyf.SendUserId], 0, 2);
    m_fileWidgets[bodyf.SendUserId]->setProgress(bodyf.TotalSize,bodyf.FileSeek + bodyf.CurrentSize,1,bodyf.FileName);
}

void GlobalUiCenter::slotRecvOnlineUserList(QString userList)
{
    m_orgMainWidget->slotRecvOnlineUserList(userList);
}

void GlobalUiCenter::slotOnlineUserUpdate(OnlineListBody body)
{
    m_orgMainWidget->slotOnlineUserUpdate(body);
}

void GlobalUiCenter::slotSendFileProgress(FileBody bodyf)
{
    m_fileWidgets[bodyf.RecvUserId]->setProgress(bodyf.TotalSize,bodyf.FileSeek,1,bodyf.FileName);
    if(bodyf.TotalSize == bodyf.FileSeek)
    {
        if(m_messageInterfaceMap.find(bodyf.RecvUserId) != m_messageInterfaceMap.end()){
            auto msgWid = m_messageInterfaceMap[bodyf.RecvUserId];
            if(msgWid){
                msgWid->addNewFileMsg(bodyf.FileName,0);
            }
        }
        //        m_messageInterface->addNewFileMsg(bodyf.FileName,0);
        MsgBody body;
        // 生成一个随机的 UUID
        QUuid uuid = QUuid::createUuid();
        // 将 UUID 转换为字符串表示形式
        QString uuidString = uuid.toString();
        body.MsgId = uuidString;
        body.Msg = bodyf.FileName;
        body.UserId = AppCache::Instance()->m_userId;
        body.SendUserId     = AppCache::Instance()->m_userId;
        body.SendUserName   = AppCache::Instance()->m_userName;
        body.DstUserId      = GlobalCenter::getInstance()->currentUserId();
        body.DstUserName    = GlobalCenter::getInstance()->currentUserName();
        body.SendTime = GlobalCenter::getInstance()->getCurrentTimeSeconds();
        body.MsgType = 2;
        body.MsgStatus = 2;
        m_sql.insertHistoryMsg(body);
    }
}

void GlobalUiCenter::slotRecvFileCompelte(FileBody bodyf)
{
    MsgBody body;
    body.UserId = bodyf.UserId;
    body.Msg = "recv file success:"+bodyf.FileName;
    m_fileWidgets[body.UserId]->hide();
}

void GlobalUiCenter::slotSendFileCompelte()
{
    //    m_fileWidget->hide();
}

void GlobalUiCenter::slotGetOrg(QJsonDocument json)
{
    m_orgMainWidget->slotGetOrg(json);
}

void GlobalUiCenter::getUserHeader(int userId, QString userName)
{
    GlobalCenter::getInstance()->slotGetUserHeadImage(userId,userName);
}

void GlobalUiCenter::drawHistoryLastMsg()
{
    auto list = m_sql.selectHistoryLastMsg();
    auto groupList = m_sql.selectHistoryLastGroupMsg();
    int index = 0;
    for(int i = 0 ; i < list.size();i++){
        auto it = list.at(i);
        QDateTime date = QDateTime::fromString(it.SendTime,"yyyy-MM-dd hh:mm:ss");
        for(index; index < groupList.size();){
            auto iter = groupList.at(index);
            QDateTime date1 = QDateTime::fromString(iter.SendTime,"yyyy-MM-dd hh:mm:ss");
            if(date < date1){
                break;
            }else{
                index++;
                bool ret = m_historicalUserList->checkUserIsExists(iter.GroupId);//判断是否已经存在了
                if(!ret){
                    m_historicalUserList->addNewUser(iter);
                }
            }
        }
        auto id = it.SendUserId == AppCache::Instance()->m_userId ? it.RecvUserId:it.SendUserId;
        auto name = it.SendUserId == AppCache::Instance()->m_userId ? it.RecvUserName:it.SendUserName;
        bool ret = m_historicalUserList->checkUserIsExists(id);//判断是否已经存在了
        if(!ret){
            m_historicalUserList->addNewUser(it);
        }

    }
    for(index; index < groupList.size();index++){
        auto iter = groupList.at(index);
        bool ret = m_historicalUserList->checkUserIsExists(iter.GroupId);//判断是否已经存在了
        if(!ret){
            m_historicalUserList->addNewUser(iter);
        }
    }
//    if(list.size() > 0){
//        auto id = list.last().SendUserId == AppCache::Instance()->m_userId ? list.last().RecvUserId:list.last().SendUserId;
//        m_historicalUserList->updateMsgInfo(id,list.last().Content,list.last().SendTime);
//    }
}

void GlobalUiCenter::slotGetUserOrg(QJsonDocument json)
{
    m_orgMainWidget->slotGetUserOrg(json);
    getUserHeader(AppCache::Instance()->m_userId,AppCache::Instance()->m_userLoginName);

    m_groupWidget->setCurrentOrgDept(m_orgMainWidget->getCurrentOrgDept());
    m_groupWidget->setCurrentOrgUser(m_orgMainWidget->getCurrentOrgUser());
    //继续获取本人的所有群组
    GlobalCenter::getInstance()->slotGetGroups();
}

void GlobalUiCenter::getMsg(QString msgid)
{
    MsgBody body;
    body.MsgId = msgid;
    body.UserId = AppCache::Instance()->m_userId;
    GlobalCenter::getInstance()->signGetMsg(body);
}

void GlobalUiCenter::slotRecvMsgNotify(MsgBody body)
{
    auto list = body.MsgId.split("|");
    for(auto it : list){
        if(!it.isEmpty())
            getMsg(it);
    }
}

void GlobalUiCenter::slotUpdateMsgStatus(MsgBody body)
{
    if(m_currentMessageInterface){
        m_currentMessageInterface->updateMsgReadStatus(body.MsgId,body.MsgStatus);
    }
    //    m_messageInterface->updateMsgReadStatus(body.MsgId,body.MsgStatus);
}

void GlobalUiCenter::slotGetOfflineNotify(MsgBody body)
{
    auto list = body.MsgId.split("|");
    for(auto it : list){
        if(!it.isEmpty())
            getMsg(it);
    }
}

void GlobalUiCenter::slotUpdateUserHeadImage(int userid, QString path)
{
    if(userid == AppCache::Instance()->m_userId){
        m_headWidget->setHeadImage(path);
        m_setDialog->updateHeadImage(path);
    }
    m_orgMainWidget->updateIcon(userid);
    m_historicalUserList->setUserHeadImage(userid);
    for(auto it : m_messageInterfaceMap){
        it->updateHead(userid);
    }
    for(auto it : m_groupInterfaceMap){
        it->updateHead(userid);
    }
}

void GlobalUiCenter::slotStartAudio(MsgBody qbody)
{
    //显示通话UI
    if(m_voiceWidget == nullptr)
    {
        m_voiceWidget = new VoiceWidget;
        connect(m_voiceWidget,&VoiceWidget::signCloseVoice,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyAudioStop);
        });
    }

    m_voiceWidget->setStatusInfo();
    m_voiceWidget->show();
    //调用方法
    GlobalCenter::getInstance()->showAudioUI(qbody);
}

void GlobalUiCenter::on_audio_btn_clicked()
{
    //1.获取MQTT 服务器地址
    if(m_voiceWidget == nullptr)
    {
        m_voiceWidget = new VoiceWidget;
        m_voiceWidget->setNickNameInfo(GlobalCenter::getInstance()->currentUserName());
        connect(m_voiceWidget,&VoiceWidget::signCloseVoice,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyAudioStop);
        });
    }

    m_voiceWidget->show();
    GlobalCenter::getInstance()->slotGetMqttServerAddr();
}

void GlobalUiCenter::on_video_btn_clicked()
{
    //打开本地摄像头 等待对方接受请求
    //显示通话UI
    if(m_videoWidget == nullptr)
    {
        m_videoWidget = new VideoWidget;
        m_videoWidget->setNickNameInfo(GlobalCenter::getInstance()->currentUserName());
        connect(m_videoWidget,&VideoWidget::signCloseVideo,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->stopReadVideo();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyVideoStop);
        });
    }

    m_videoWidget->show();
    GlobalCenter::getInstance()->slotGetVideoMqttServerAddr();
}

void GlobalUiCenter::slotInviteAudio(MsgBody body)
{
    //显示邀请UI,选择拒绝或者接受
    if(m_inviteWidget == nullptr)
    {
        m_inviteWidget = new InviteWidget;
    }

    m_inviteWidget->setInviteType(0);
    m_inviteWidget->setStatusInfo(QStringLiteral("邀请你语音通话..."));
    m_inviteWidget->show();
    m_inviteWidget->recvMsgBody(body);
}

void GlobalUiCenter::slotAccptAudio(MsgBody body)
{
    //显示通话UI
    if(m_voiceWidget == nullptr)
    {
        m_voiceWidget = new VoiceWidget;
        connect(m_voiceWidget,&VoiceWidget::signCloseVoice,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyAudioStop);
        });
    }
    m_voiceWidget->raise();
    //    m_videoWidget->show();
    m_voiceWidget->setStatusInfo();
}

void GlobalUiCenter::slotInviteVideo(MsgBody body)
{
    //显示邀请UI,选择拒绝或者接受
    if(m_inviteWidget == nullptr)
    {
        m_inviteWidget = new InviteWidget;
    }

    m_inviteWidget->setInviteType(1);
    m_inviteWidget->setStatusInfo(QStringLiteral("邀请你视频通话..."));
    m_inviteWidget->show();
    m_inviteWidget->recvMsgBody(body);
}

void GlobalUiCenter::slotStartVideo(MsgBody body)
{
    //显示通话UI
    if(m_videoWidget == nullptr)
    {
        m_videoWidget = new VideoWidget;
        connect(m_videoWidget,&VideoWidget::signCloseVideo,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->stopReadVideo();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyVideoStop);
        });
    }

    m_videoWidget->setStatusInfo();
    m_videoWidget->setConnectStatus(1);
    m_videoWidget->show();
    //调用方法
    GlobalCenter::getInstance()->showVideoUI(body);
}

void GlobalUiCenter::slotAccptVideo(MsgBody body)
{
    //显示通话UI
    if(m_videoWidget == nullptr)
    {
        m_videoWidget = new VideoWidget;
        connect(m_videoWidget,&VideoWidget::signCloseVideo,[=]{
            GlobalCenter::getInstance()->signStopRun();
            GlobalCenter::getInstance()->stopReadVideo();
            GlobalCenter::getInstance()->sendAudioMsg("",NotifyVideoStop);
        });
    }
    m_videoWidget->raise();
    m_videoWidget->setConnectStatus(1);
    //    m_videoWidget->show();
    m_videoWidget->setStatusInfo();
}

void GlobalUiCenter::slotCloseVoice()
{
    if(m_voiceWidget){
        GlobalCenter::getInstance()->signStopRun();
        m_voiceWidget->setSignSw(0);
        m_voiceWidget->close();
    }
}

void GlobalUiCenter::slotCloseVideo()
{
    if(m_videoWidget){
        GlobalCenter::getInstance()->signStopRun();
        GlobalCenter::getInstance()->stopReadVideo();
        m_videoWidget->setSignSw(0);
        m_videoWidget->close();
    }
}

//本地摄像头图像
void GlobalUiCenter::slotImageReadReady(QImage img)
{
    //显示通话UI
    if(m_videoWidget == nullptr)
    {
        return;
    }
    m_videoWidget->paintLocalVideo(img);
}
//远程摄像头
void GlobalUiCenter::slotRemoteImageReadReady(QImage img)
{
    if(m_videoWidget == nullptr)
    {
        return;
    }
    m_videoWidget->paintRemoteVideo(img);
}

void GlobalUiCenter::shakeWindow() {

    m_mainWindow->setWindowState(m_mainWindow->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    m_mainWindow->activateWindow();
    m_mainWindow->raise();
    m_mainWindow->activateWindow();
    // 创建动画对象，指定要动画化的属性为几何属性
    QPropertyAnimation *animation = new QPropertyAnimation(m_mainWindow, "geometry");

    // 设置动画持续时间
    animation->setDuration(500);

    // 获取当前窗口位置和大小
    QRect geometry = m_mainWindow->geometry();
    int x = geometry.x();
    int y = geometry.y();
    int w = geometry.width();
    int h = geometry.height();

    // 设置动画路径，通过改变位置实现抖动
    animation->setKeyValueAt(0, QRect(x, y, w, h));
    animation->setKeyValueAt(0.1, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.2, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.3, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.4, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.5, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.6, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.7, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.8, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.9, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(1, QRect(x, y, w, h));

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GlobalUiCenter::msgListWidgetItemClicked(int userId,QString userName)
{
    // 逆序遍历，安全删除元素
    for (int i = m_messageInterface->count() - 1; i >= 0; --i) {
        auto item = m_messageInterface->itemAt(i);
        if (item->widget()) { // 确保这是一个小部件
            auto widget = item->widget();

            // 检查这是否为QPushButton实例
            if (qobject_cast<MessageInterface *>(widget)) {
                widget->hide();
                //                m_messageInterface->removeItem(item); // 从布局中移除
            }
        }
    }

    if(m_messageInterfaceMap.find(userId) == m_messageInterfaceMap.end()){//只在第一次点击的时候 载入一次数据库数据
        m_currentMessageInterface  = new MessageInterface();
        m_messageInterfaceMap[userId] = m_currentMessageInterface;
        m_messageInterface->addWidget(m_currentMessageInterface);
        m_messageInterfaceMap[userId]->listWidgetItemClicked(userId,userName);


    }else{
        GlobalCenter::getInstance()->setCurrentUser(userId,userName);
        m_currentMessageInterface = m_messageInterfaceMap[userId];
        m_currentMessageInterface->show();
        //        m_messageInterface->addWidget(m_currentMessageInterface);
    }

    //    m_currentMessageInterface->listWidgetItemClicked(userId,userName);
}
