#ifndef GLOBALUICENTER_H
#define GLOBALUICENTER_H

#include <QGridLayout>
#include <QObject>
#include <QStackedWidget>
#include "mainwindow.h"
#include "Gui/ChatUi/historical_user_list.h"
#include "Gui/ChatUi/message_interface.h"
#include "Gui/ChatUi/send_text_edit.h"
#include "Gui/HeadUi/head_widget.h"
#include "Gui/OrgUi/org_main_widget.h"
#include "Gui/HeadUi/head_widget.h"
#include "Gui/GroupUi/group_main_widget.h"
#include <QLayout>

class GlobalUiCenter : public QObject
{
    Q_OBJECT
public:
    static GlobalUiCenter * getInstance(){
        return _instance;
    }

    void setMainWindow(MainWindow * ptr){
        m_mainWindow = ptr;
    }
    void setMessageInterfaceLayout(QVBoxLayout * ptr){
        m_messageInterface = ptr;
    }
    void setSendTextEdit(SendTextEdit * ptr){
        m_sendTextEdit = ptr;
    }
    void setOrgMainWidget(OrgMainWidget * ptr){
        m_orgMainWidget = ptr;
    }
    void setHeadWidget(HeadWidget * ptr){
        m_headWidget = ptr;
    }
    void setHistoricalUserList(HistoricalUserList * ptr){
        m_historicalUserList = ptr;
    }
    void setFileGridLayout(QGridLayout * ptr){
        m_fileGridLayout = ptr;
    }
    void setStackedWidget(QStackedWidget * ptr){
        m_stackedWidget =   ptr;
    }
    void setGroupWidget(GroupMainWidget * ptr){
        m_groupWidget =   ptr;
    }
    void initUi();
public slots:
    //----------------音视频相关----------------
    void slotStartAudio(MsgBody qbody);
    void slotAccptAudio(MsgBody body);
    void slotInviteVideo(MsgBody body);
    void slotStartVideo(MsgBody body);
    void slotAccptVideo(MsgBody body);
    void slotInviteAudio(MsgBody body);
    void slotCloseVoice();
    void slotCloseVideo();
    void slotImageReadReady(QImage img);
    void slotRemoteImageReadReady(QImage img);
    void on_audio_btn_clicked();
    void on_video_btn_clicked();
    //----------------登录相关----------------
    void slotLoginStatus(int status, QString str);
    void slotLoginBody(LoginBody body);
    //----------------消息相关----------------
    void slotRecvMsg(MsgBody body);
    void slotRecvMsgNotify(MsgBody body);
    void slotUpdateMsgStatus(MsgBody body);
    void msgListWidgetItemClicked(int userId, QString userName);
    void slotGetOfflineNotify(MsgBody body);
    void shakeWindow();
    void on_sendmsg_btn_clicked();
    void slotHasImag();
    // ----------------群组相关----------------
    void createGroup(GroupBody);
    void slotCreateGroup(GroupBody);
    void groupListWidgetItemClicked(QListWidgetItem *item);
    void groupListWidgetItemClicked(QString groupId);
    void groupDoubleClicked(const QModelIndex &index);
    void groupClicked(const QModelIndex &index);
    void slotRecvGroups(QList<GroupStruct>);
    void slotRecvGroupUsers(QList<GroupUsersStruct>);
    //----------------文件相关----------------
    void slotRecvFileProgress(FileBody bodyf);
    void slotSendFileProgress(FileBody bodyf);
    void slotRecvFileCompelte(FileBody bodyf);
    void slotCurrentUserListUpdate(QList<int> list);
    void slotSendFileCompelte();
    void on_file_btn_clicked();
    void slotCloseFile(FileBody);
    //----------------在线状态相关----------------
    void slotRecvOnlineUserList(QString userList);
    void slotOnlineUserUpdate(OnlineListBody body);
    //----------------组织架构相关----------------
    void slotGetOrg(QJsonDocument json);
    void slotGetUserOrg(QJsonDocument json);
    //----------------头像相关----------------
    void slotUpdateUserHeadImage(int userid, QString path);
    //----------------其他----------------
    void doubleClicked(const QModelIndex &index);
    void clicked(const QModelIndex &index);
    void on_set_btn_clicked();
    void on_msg_listWidget_itemClicked(QListWidgetItem *item);
signals:
    void signUpdataUnread(int id,int number);
private:
    static GlobalUiCenter* _instance;
    explicit GlobalUiCenter(QObject *parent = nullptr);

    void drawHistoryLastMsg();
    void getMsg(QString msgid);
    void getUserHeader(int userId, QString userName);
private:
    MainWindow *        m_mainWindow        = nullptr;
    MessageInterface *  m_currentMessageInterface = nullptr;
    QVBoxLayout *       m_messageInterface  = nullptr;
    QMap<int,MessageInterface*> m_messageInterfaceMap;
    QMap<QString,MessageInterface*> m_groupInterfaceMap;
    SendTextEdit *      m_sendTextEdit      = nullptr;
    OrgMainWidget *     m_orgMainWidget     = nullptr;
    HeadWidget *        m_headWidget        = nullptr;
    HistoricalUserList *m_historicalUserList= nullptr;
    QGridLayout *       m_fileGridLayout    = nullptr;
    QStackedWidget *    m_stackedWidget     = nullptr;
    EmotionWidget *     m_emojiWidget       = nullptr;
    VoiceWidget*        m_voiceWidget       = nullptr;
    VideoWidget*        m_videoWidget       = nullptr;
    InviteWidget*       m_inviteWidget      = nullptr;
    SettingDialog       *m_setDialog        = nullptr;
    GroupMainWidget     *m_groupWidget      = nullptr;
    Sqlite              m_sql;
    QMap<int,FileWidget*>           m_fileWidgets;
    int                 m_currentChatType;
};

#endif // GLOBALUICENTER_H
