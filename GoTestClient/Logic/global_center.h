#ifndef GLOBALCENTER_H
#define GLOBALCENTER_H

#include <QObject>
#include "Network/LhtTcpClient/lht_tcp_client.h"
#include "utils/log/QsLog.h"
#include "global_config.h"
#include "utils/voice/audio_play.h"
#include "utils/video/vedio_play.h"
/**
 * @brief 全局转发中心 连接UI和数据层
 */

class GlobalCenter : public QObject
{
    Q_OBJECT
public:
    static GlobalCenter * getInstance(){
        return _instance;
    }

    void initTcp(QString  ip,int port);

    void setCurrentUser(int userId, QString userName);

    void setCurrentUserId(int userId);

    void setCurrentUserName( QString userName);

    void setCurrentGroupId( QString groupId);

    void initConnection();

    QString getCurrentTimeSeconds();

    QString getHostMacAddress();

    void openLocalDevice();

    int getLoginStatus(){
        return m_loginStatus;
    }

    int currentUserId(){
        return m_currentChoiseUserId;
    }
    QString currentGroupId(){
        return m_currentChoiseGroup;
    }
    QString currentUserName(){
        return m_currentChoiseUser;
    }
    //----------------------Tcp相关------------------------
    MsgBody sendMsg(QString msg,int msgType = 0,int method = SendMsg);

    GroupBody sendGroupMsg(QString msg,int msgType = 0,int method = SendGroupMsg);

    MsgBody sendAudioMsg(QString mqttServer,int msgType = NotifyAudioStart);

    MsgBody sendVideoMsg(QString mqttServer,int msgType = NotifyVideoStart);
    //---------------------- 音视频相关------------------------
    void initAudio();

    void initVideo();

    void openAudio(MsgBody body);

    void openVideo(MsgBody body);

    void startReadLocal();

    void startCommunication();

    void closeAudio();

    void closeVideo();

    void showVideoUI(MsgBody qbody);

    void showAudioUI(MsgBody qbody);
    //---------------------- 群组相关------------------------
    void createGroup(GroupBody body);

    void slotGetGroups();

    void slotGetGroupUsers(QString groupId);
private:
    static GlobalCenter* _instance;
    explicit GlobalCenter(QObject *parent = nullptr);
    //Tcp实例
    LhtTcpClient   *   m_tcpClient;
    //登录状态
    int             m_loginStatus = 0 ;

    QString         m_currentChoiseUser;
    QString         m_currentChoiseGroup;
    //当前UI选中的用户ID
    int             m_currentChoiseUserId;

    //文件实例
    LhtFileClient   *   m_fileClient;

    QMap<QString,MsgBody>           m_md52msg;

    QMap<QString,GroupBody>           m_md52groupMsg;

    QMap<int,QMap<QString,MsgBody>>           m_recvMd52msg;
    QList<int> m_allUserId;
    QList<int> m_allUserIdTemp;

    VedioPlay   *   m_videoPlay = nullptr;
    AudioInput *    m_audioPlay = nullptr;
signals:
    void signStartRun(QString ip , int port,int pushId,int recvId);

    void signStopRun();

    void openDevice(const QString &,QString ip , int port,int pushId,int recvId);

    void startRead();

    void stopReadVideo();

    void signCloseFile(FileBody);

    void signImageReadReady(QImage);

    void signRemoteImageReadReady(QImage);

    void signLogin(QString  ip,int port ,LoginBody body);

    void signUpdateStatus(LoginBody body,int port ,int );

    void signLogout(LoginBody body);

    void signSendMsg(MsgBody body, int method ,int methodType);

    void signGetMqttServerAddr(SystemBody body);

    void signGetVideoMqttServerAddr(SystemBody body);

    void signGetMsg(MsgBody body);

    void signGetFileAddr(FileBody body, int method ,int methodType);

    void signGetOrg(SystemBody body, int method ,int methodType);

    void signOnlineUsers(SystemBody body, int method ,int methodType);

    void signGetUserHeadImage(SystemBody body, int method ,int methodType);

    void signGetOfflineMsg(SystemBody body);

    void signUpdateMsgStatus(MsgBody);

    void signMsgHadBeenRead(MsgBody);

    void signSendFileRequest(QString path);

    void signStartRecvFile(FileBody body);

    void signStartSendFile(FileBody body);

    void signUploadUserHeadImage(FileBody body);

    void signUploadUserHeadImageData(FileBody body);

    void signUploadChatImage(FileBody body);

    void signUploadChatImageData(FileBody body);

    void signDownloadChatImage(FileBody body);

    void signDownloadChatImageData(FileBody body);

    void signUpdateConnectFileServerState(FileBody body);

    void signSendFileData(int,FileBody);

    void signSendGroupMsg(GroupBody body,int method,int methodType = 0);

    void signFileCancel(FileBody);

    void signUpdateUserHeadImage( int,  QString);

    void signUploadHeadImageState(bool success);

    void signJitter(MsgBody body);

    void signStartAudio(MsgBody body);

    void signAccptAudio(MsgBody body);

    void signStartVideo(MsgBody body);

    void signAccptVideo(MsgBody body);

    void signStopAudio();

    void signStopVideo();

    void signCreateGroup(GroupBody body);

    void signRecvGroups(QList<GroupStruct>);

    void signRecvGroupUsers(QList<GroupUsersStruct>);
public slots:

    void slotLoginStatus(int status,QString str);

    void slotGetUserHeadImage(int userId,QString userName);

    void slotRecvMsg(MsgBody);

    void slotUpdateUserHeadImage(int userId,QString path);

    void slotRecvMsgNotify(MsgBody);

    void slotUpdateMsgStatus(MsgBody);

    void slotGetOfflineNotify(MsgBody);

    void slotRecvOnlineUserList(QString userList);

    void slotOnlineUserUpdate(OnlineListBody body);

    void slotRecvFileCompelte(FileBody bodyf);

    void slotSendFileCompelte();

    void slotCreateGroup(GroupBody body);

    void slotFileCancel(FileBody);

    void slotLoginBody(LoginBody);

    void slotGetOrg(QJsonDocument json);

    void slotGetUserOrg(QJsonDocument json);

    void slotRecvGroups(QList<GroupStruct>);

    void slotRecvGroupUsers(QList<GroupUsersStruct>);

    void slotRecvFileProgress(FileBody body);

    void slotSendFileProgress(FileBody body);

    void slotStartAudio(MsgBody body);

    void slotAccptAudio(MsgBody body);

    void slotStartVideo(MsgBody body);

    void slotAccptVideo(MsgBody body);

    void initDevice(MsgBody body);

    void slotFileCmd(QString path,bool type,int cmd);

    void initLog(QsLogging::Level level , const QString &logPath, long long maxSizeBytes, int maxOldLogCount);

    void setAllUserId(QList<int>);

    void slotJitter( );

    void slotSendMsg(QString uuid);

    void slotGetMqttServerAddr();

    void slotGetVideoMqttServerAddr();

    void slotRecvMsgImage(int userId , QString fileName);

signals:
    void signTpcConnected();

    void signLoginStatus(int status,QString str = "");

    void signLoginBody(LoginBody);

    void signOnlineUserList(QString userList);

    void signOnlineUserUpdate(OnlineListBody body);
    //500ms 向界面发送一次包防止一直刷新卡顿
    void signRecvMsgList(QList<MsgBody>);

    void signRecvMsg(MsgBody);

    void signRecvMsgNotify(MsgBody);

    void signGetOfflineNotify(MsgBody);


    //发送到 文件线程
    void signRecvFile(FileBody);

    void signContinueSendFile(int,FileBody);

    void signSendFileProgress(FileBody bodyf);

    void signSendFileCompelte();

    void signRecvFileProgress(FileBody body);

    void signRecvFileCompelte(FileBody bodyf);

    void signGetOrgRes(QJsonDocument body);

    void signGetUserOrg(QJsonDocument body);

    void signSetFileSendName(QString);

    void signFileServer(QString ip,int port);
};

#endif // GLOBALCENTER_H
