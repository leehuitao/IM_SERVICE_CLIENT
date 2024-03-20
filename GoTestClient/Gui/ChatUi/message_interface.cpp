#include "message_interface.h"
#include "ui_message_interface.h"
//#include <WinUser.h>
//#include <Winsock2.h>
#include <QScrollBar>

MessageInterface::MessageInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageInterface)
{
    ui->setupUi(this);
    ui->listWidget->setStyleSheet("QListWidget::item:selected { background-color: transparent; }"
                                  "QListWidget::item:hover { background-color: transparent; }"
                                  "QListWidget::item:focus {outline: none;}");
    initDB();
    this->setMinimumWidth(300);

}
void MessageInterface::initDB()
{
    m_sql.initDB();
}

void MessageInterface::setWinId(HWND id)
{
    m_mainWinId = id;
}
MessageInterface::~MessageInterface()
{
    delete ui;
}

void MessageInterface::addShakeMsg(const QString &msg)
{
    MessageBoxItem *w = new MessageBoxItem();
    w->initMsg(msg,5);
    w->setFixedWidth(this->width());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(w->getSize());
    ui->listWidget->setItemWidget(item, w);
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
}

void MessageInterface::addNewMsg(const MsgBody &msg)
{

    if(msg.Msg.contains(ImageHeader)){
        MessageBoxItem *w = new MessageBoxItem();
        auto temp = msg;
        temp.Msg.replace(ImageHeader,"");
        temp.Msg.replace(ImageEnd,"");
        w->initImageMsg(temp);
        w->setFixedWidth(this->width());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(w->getSize());
        ui->listWidget->setItemWidget(item, w);
        //        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
        vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
    }else{
        if(msg.Msg != " " && !msg.Msg.isEmpty()){
            MessageBoxItem *w = new MessageBoxItem();
            w->initMsg(msg);
            auto size = this->width();
            w->setFixedWidth(size);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            item->setSizeHint(w->getSize());
            ui->listWidget->setItemWidget(item, w);
            //            ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
            vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
        }

    }
}

void MessageInterface::addNewGroupMsg(const GroupBody &msg)
{
    if(this->isHidden()&& msg.MsgStatus == 1&&msg.SendUserId != AppCache::Instance()->m_userId)
        m_unreadGroupMsgIdList.append(msg);
    if(msg.Msg.contains(ImageHeader)){
        MessageBoxItem *w = new MessageBoxItem();
        auto temp = msg;
        temp.Msg.replace(ImageHeader,"");
        temp.Msg.replace(ImageEnd,"");
        w->initGroupImageMsg(temp);
        w->setFixedWidth(this->width());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(w->getSize());
        ui->listWidget->setItemWidget(item, w);
        //        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
        vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
    }else{
        if(msg.Msg != " " && !msg.Msg.isEmpty()){
            MessageBoxItem *w = new MessageBoxItem();
            w->initGroupMsg(msg);
            auto size = this->width();
            w->setFixedWidth(size);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            item->setSizeHint(w->getSize());
            ui->listWidget->setItemWidget(item, w);
            //            ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
            vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
        }

    }
}

void MessageInterface::setMaxiMum()
{
    // 将垂直滚动条设置为最大值，将滚动条滚动到底部
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    QScrollBar *vScrollBar = ui->listWidget->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum()); // 滚动到底部
}

void MessageInterface::addNewFileMsg(const QString &filePath, int type)
{
    MessageBoxItem *w = new MessageBoxItem();
    w->initMsg(filePath,type);
    w->setFixedWidth(this->width());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(w->getSize());
    ui->listWidget->setItemWidget(item, w);
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}
void MessageInterface::flashWindow() {
    QApplication::alert(this);
    //    FLASHWINFO fwInfo;
    //    fwInfo.cbSize = sizeof(FLASHWINFO);
    //    fwInfo.hwnd = reinterpret_cast<HWND>(m_mainWinId);
    //    fwInfo.dwFlags = FLASHW_ALL;
    //    fwInfo.uCount = 2; // 闪烁次数
    //    fwInfo.dwTimeout = 0;

    //    FlashWindowEx(&fwInfo);
}

void MessageInterface::slotRecvMsg(MsgBody body)
{
    if(body.MsgType == 4){
        if(body.SendUserId != AppCache::Instance()->m_userId){
            addShakeMsg(ShakeRecv);
        }
    }else{
        if(body.SendUserId == AppCache::Instance()->m_userId){//表示发送消息成功了，插入本地数据库，更新消息状态为未读
            updateMsgReadStatus(body.MsgId,body.MsgStatus);
        }else if(body.DstUserId == AppCache::Instance()->m_userId){//接收到消息了
            if(this->isHidden() && body.MsgStatus == 1)
                m_unreadMsgIdList.append(body);
            m_sql.insertHistoryMsg(body);
            if(GlobalCenter::getInstance()->currentUserId() == body.SendUserId){
                addNewMsg(body);
                updateMsgReadStatus(body.MsgId,2);
                body.MsgStatus = 2;
                body.UserId = AppCache::Instance()->m_userId;
                //            slotUpdateMsgStatus(body);
                GlobalCenter::getInstance()->signUpdateMsgStatus(body);
                if (!this->isActiveWindow()) {
                    flashWindow();
                }
            }else{
                flashWindow();//其他人的话就闪两下图标
            }
        }
    }

}

void MessageInterface::updateMsgReadStatus(QString msgId, int status)
{
    QList<MessageBoxItem*> tlist = ui->listWidget->findChildren<MessageBoxItem*>();

    for(auto it : tlist){
        if(it->getMsgId() == msgId){
            it->updateMsgReadStatus(status);
            //            break;
        }
    }

    m_sql.updateMsgStatus(msgId,status);
}

void MessageInterface::updateGroupMsgReadStatus(QString msgId, int status)
{
    m_sql.updateGroupMsgStatus(msgId,status);
}

void MessageInterface::updateHead(int userId)
{
    QList<MessageBoxItem*> tlist = ui->listWidget->findChildren<MessageBoxItem*>();

    for(auto it : tlist){
        if(it->getMsgBody().SendUserId == userId){
            it->setUserAvatar(userId);
            //            break;
        }
    }
}

void MessageInterface::clear()
{
    ui->listWidget->clear();
}

void MessageInterface::showEvent(QShowEvent *event)
{
    //    setMaxiMum();
    for(auto it : m_unreadMsgIdList){
        updateMsgReadStatus(it.MsgId,2);
        it.MsgStatus = 2;
        //            m_sql.updateMsgStatus(body.MsgId,body.MsgStatus);
        GlobalCenter::getInstance()->signUpdateMsgStatus(it);
    }
    m_unreadMsgIdList.clear();
    for(auto it : m_unreadGroupMsgIdList){
        updateGroupMsgReadStatus(it.MsgId,2);
        it.MsgStatus = 2;
        GlobalCenter::getInstance()->signUpdateGroupMsgStatus(it);
    }
    m_unreadGroupMsgIdList.clear();

}

void MessageInterface::resizeEvent(QResizeEvent *event)
{
    QList<MessageBoxItem*> tlist = ui->listWidget->findChildren<MessageBoxItem*>();

    int index = 0;
    for(auto it : tlist){
        it->setFixedWidth(this->width());
        auto iter = ui->listWidget->item(index++);
        if(iter)
            iter->setSizeHint(it->getSize());
    }
}

void MessageInterface::listWidgetItemClicked(QListWidgetItem *item)
{
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
                GlobalCenter::getInstance()->setCurrentUser(currentChoiseUserId,currentChoiseUser);
                wid->clearUnread();
                wid->updateUnread(0);
                auto list = m_sql.selectHistoryMsg(currentChoiseUserId,AppCache::Instance()->m_userId);
                clear();
                for(auto it : list){
                    MsgBody body(it);
                    if(body.MsgType == 0)
                        addNewMsg(body);
                    else
                        addNewFileMsg(body.Msg,body.MsgType == 1 ? 1 : 0);
                    if(body.MsgStatus == 1 && body.DstUserId == AppCache::Instance()->m_userId){//如果时未读消息
                        body.UserId = AppCache::Instance()->m_userId;
                        body.MsgStatus = 2;
                        updateMsgReadStatus(body.MsgId,body.MsgStatus);
                        m_sql.updateMsgStatus(body.MsgId,body.MsgStatus);
                        GlobalCenter::getInstance()->signUpdateMsgStatus(body);
                    }
                }

                //                qDebug()<<index.data()<<index.data(Qt::WhatsThisRole);
                //                qDebug() << "Clicked on QPushButton: " << wid->getUserId();
            }
        }
    }
}

void MessageInterface::listWidgetItemClicked(int userId, QString userName)
{
    if(GlobalCenter::getInstance()->currentUserId() == userId)
        return;
    GlobalCenter::getInstance()->setCurrentUser(userId,userName);

    auto list = m_sql.selectHistoryMsg(userId,AppCache::Instance()->m_userId);
    //    clear();
    for(auto it : list){
        MsgBody body(it);
        if(body.MsgType == 0)
            addNewMsg(body);
        else
            addNewFileMsg(body.Msg,body.MsgType == 1 ? 1 : 0);
        if(body.MsgStatus == 1 && body.DstUserId == AppCache::Instance()->m_userId){//如果时未读消息
            body.UserId = AppCache::Instance()->m_userId;
            body.MsgStatus = 2;
            updateMsgReadStatus(body.MsgId,body.MsgStatus);
            //            m_sql.updateMsgStatus(body.MsgId,body.MsgStatus);
            GlobalCenter::getInstance()->signUpdateMsgStatus(body);
        }
    }
}

void MessageInterface::listWidgetItemClicked(QString groupId)
{
    if(GlobalCenter::getInstance()->currentGroupId() == groupId)
        return;
    GlobalCenter::getInstance()->setCurrentGroupId(groupId);

    auto list = m_sql.selectHistoryMsg(groupId);
    for(auto it : list){
        GroupBody body(it);
        if(body.MsgType == 0)
            addNewGroupMsg(body);
        else
            addNewFileMsg(body.Msg,body.MsgType == 1 ? 1 : 0);
    }
}

