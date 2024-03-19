#include "historical_user_list.h"
#include "Logic/global_center.h"
HistoricalUserList::HistoricalUserList(QWidget *parent):QListWidget(parent)
{

}

bool HistoricalUserList::checkUserIsExists(int userId)
{
    if(m_userid2UserChat.find(userId) != m_userid2UserChat.end())
        return 1;
    else
        return 0;
}

bool HistoricalUserList::checkUserIsExists(QString groupId)
{
    if(m_groupid2UserChat.find(groupId) != m_groupid2UserChat.end())
        return 1;
    else
        return 0;
}

void HistoricalUserList::addNewUser(int userId, QString userName)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, userId);
    this->insertItem(0,item);
    auto w = new MsgWidgetItem;

    m_userid2UserChat[userId] =  w;
    signCurrentUserListUpdate(m_userid2UserChat.keys());
    auto path = AppCache::Instance()->m_headImagePath[userId];
    w->setUserInfo(userName,userId,path.isEmpty() ?m_headImagePath:path);
    item->setSizeHint(w->minimumSizeHint());
    this->setItemWidget(item,w);
    this->setCurrentItem(item);
}

void HistoricalUserList::addNewUser(QString groupId, QString userName)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, groupId);
    this->insertItem(0,item);
    auto w = new MsgWidgetItem;

    m_groupid2UserChat[groupId] =  w;
    signCurrentUserListUpdate(m_userid2UserChat.keys());
    w->setUserInfo(userName,groupId,m_headImagePath);
    item->setSizeHint(w->minimumSizeHint());
    this->setItemWidget(item,w);
    this->setCurrentItem(item);
}

void HistoricalUserList::addNewUser(const MsgBody &body)
{
    int userId ;
    QString userName;

    if(body.SendUserId == AppCache::Instance()->m_userId){
        LoginBody userInfo = AppCache::Instance()->m_userInfos[body.DstUserId];
        userId = body.DstUserId;
        userName = userInfo.UserName;
    }else{
        LoginBody userInfo = AppCache::Instance()->m_userInfos[body.SendUserId];
        userId = body.SendUserId;
        userName = userInfo.UserName;
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, userId);
    this->insertItem(0,item);
    auto w = new MsgWidgetItem;

    m_userid2UserChat[userId] =  w;

    signCurrentUserListUpdate(m_userid2UserChat.keys());
   

    auto path = AppCache::Instance()->m_headImagePath[userId];
    w->setUserInfo(userName,userId,path.isEmpty() ?m_headImagePath:path);

    w->updateMsgInfo(body.Msg,body.SendTime);
    item->setSizeHint(w->minimumSizeHint());
    this->setItemWidget(item,w);
    this->setCurrentItem(item);
}

void HistoricalUserList::updateMsgInfo(int userId, const QString &msg, const QString &time)
{
    m_userid2UserChat[userId]->updateMsgInfo(msg,time);
}

void HistoricalUserList::updateMsgItemSort(int userId)
{
    QString name;
    int id;
    QString path;
    for (int i = 0; i < this->count(); i++) {
        QListWidgetItem *currentItem =this->item(i);
        auto specialLabel = currentItem->data(Qt::UserRole).toInt();
        if (specialLabel == userId) {
            // 移除匹配的项
            QWidget *widgetItem = this->itemWidget(currentItem);
            MsgWidgetItem *wid = qobject_cast<MsgWidgetItem*>(widgetItem);
            name = wid->getUserName();
            id = wid->getUserId();
            path = wid->getAvatar();
            this->takeItem(i);
            // 将匹配的项插入到首位
            break; // 假定只有一个匹配项
        }
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, GlobalCenter::getInstance()->currentUserId());
    this->insertItem(0,item);
    auto w = new MsgWidgetItem;
    m_userid2UserChat[GlobalCenter::getInstance()->currentUserId()] =  w;
    signCurrentUserListUpdate(m_userid2UserChat.keys());
    w->setUserInfo(name,id,path);
    item->setSizeHint(w->minimumSizeHint());
    this->setItemWidget(item,w);
    this->setCurrentItem(item);
}

void HistoricalUserList::updateMsgItemSort(QString groupId)
{
    QString group;
    QString groupName;
    QString path;
    for (int i = 0; i < this->count(); i++) {
        QListWidgetItem *currentItem =this->item(i);
        auto specialLabel = currentItem->data(Qt::UserRole).toString();
        if (specialLabel == groupId) {
            // 移除匹配的项
            QWidget *widgetItem = this->itemWidget(currentItem);
            MsgWidgetItem *wid = qobject_cast<MsgWidgetItem*>(widgetItem);
            group = wid->getGroupId();
            groupName = wid->getGroupName();
            path = wid->getAvatar();
            this->takeItem(i);
            // 将匹配的项插入到首位
            break; // 假定只有一个匹配项
        }
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, GlobalCenter::getInstance()->currentUserId());
    this->insertItem(0,item);
    auto w = new MsgWidgetItem;
    m_groupid2UserChat[GlobalCenter::getInstance()->currentGroupId()] =  w;
//    signCurrentUserListUpdate(m_groupid2UserChat.keys());  //文件UI添加
    w->setUserInfo(groupName,groupId,path);
    item->setSizeHint(w->minimumSizeHint());
    this->setItemWidget(item,w);
    this->setCurrentItem(item);
}

void HistoricalUserList::setCurrentUser(int userId)
{
    for (int i = 0; i < this->count(); i++) {
        QListWidgetItem *currentItem =this->item(i);
        auto specialLabel = currentItem->data(Qt::UserRole).toInt();
        if (specialLabel == userId) {
            this->setCurrentItem(currentItem);
            break;
        }
    }
}

void HistoricalUserList::setCurrentGroup(QString groupId)
{
    for (int i = 0; i < this->count(); i++) {
        QListWidgetItem *currentItem =this->item(i);
        auto specialLabel = currentItem->data(Qt::UserRole).toString();
        if (specialLabel == groupId) {
            this->setCurrentItem(currentItem);
            break;
        }
    }
}

void HistoricalUserList::setUserHeadImage(int userId)
{
    if(m_userid2UserChat.contains(userId)){
        auto item = m_userid2UserChat[userId];
        if(item){
            item->setUserHead(AppCache::Instance()->m_headImagePath[userId]);
        }
    }
}

void HistoricalUserList::slotUpdataUnread(int id, int number)
{
    auto keys = m_userid2UserChat.keys();
    for(auto it: keys)
    {
        if(it == id){
            m_userid2UserChat[it]->updateUnread(number);
        }

    }
}

void HistoricalUserList::slotUpdataUnread(QString id, int number)
{
    auto keys = m_groupid2UserChat.keys();
    for(auto it: keys)
    {
        if(it == id){
            m_groupid2UserChat[it]->updateUnread(number);
        }

    }
}
