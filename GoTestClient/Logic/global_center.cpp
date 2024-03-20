#include "global_center.h"

#include <QCameraInfo>
#include <QNetworkInterface>
#include <QUuid>

GlobalCenter* GlobalCenter::_instance = new  GlobalCenter;

void GlobalCenter::initTcp(QString ip, int port)
{
    //    if(m_loginStatus == 0){
    LoginBody body;
    body.UserLoginName   = AppCache::Instance()->m_userLoginName;
    body.PassWord       = AppCache::Instance()->m_passWord;
    body.LoginTime      = getCurrentTimeSeconds();
    body.MacAddress     = getHostMacAddress();
    body.Notice         = 0;
    signLogin(ip,port,body);
    //    }else{

    //    }
}

void GlobalCenter::setCurrentUser(int userId, QString userName)
{
    m_currentChoiseUserId = userId;
    m_currentChoiseUser = userName;
}

void GlobalCenter::setCurrentUserId(int userId)
{
    m_currentChoiseUserId = userId;
}

void GlobalCenter::setCurrentUserName(QString userName)
{
    m_currentChoiseUser = userName;
}

void GlobalCenter::setCurrentGroupId(QString groupId)
{
    m_currentChoiseGroup = groupId;
}

void GlobalCenter::initConnection()
{
    //日志初始化

    m_tcpClient = new LhtTcpClient;
    m_fileClient = new LhtFileClient;
    QThread *t1 = new QThread;
    QThread *t2 = new QThread;
    connect(this,&GlobalCenter::signLogin,                m_tcpClient,&LhtTcpClient::sendLogin                 ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUpdateStatus,         m_tcpClient,&LhtTcpClient::sendUpdateStatus          ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signLogout,               m_tcpClient,&LhtTcpClient::sendLogout                ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signSendMsg,              m_tcpClient,&LhtTcpClient::sendMsg                   ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetMqttServerAddr,    m_tcpClient,&LhtTcpClient::slotGetMqttServerAddr                   ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetVideoMqttServerAddr,    m_tcpClient,&LhtTcpClient::slotGetVideoMqttServerAddr                   ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetFileAddr,          m_tcpClient,&LhtTcpClient::slotGetFileServerAddr         ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetOrg,               m_tcpClient,&LhtTcpClient::sendGetOrg                ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signOnlineUsers,          m_tcpClient,&LhtTcpClient::sendGetOnlineUsers        ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetUserHeadImage,     m_tcpClient,&LhtTcpClient::sendGetUserHeadImage        ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetMsg,               m_tcpClient,&LhtTcpClient::sendGetMsg                ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetOfflineMsg,        m_tcpClient,&LhtTcpClient::sendGetOfflineMsg         ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUpdateMsgStatus,      m_tcpClient,&LhtTcpClient::sendUpdateMsgStatus       ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUploadUserHeadImage,        m_tcpClient,&LhtTcpClient::slotUploadUserHeadImage      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUploadUserHeadImageData,    m_tcpClient,&LhtTcpClient::slotUploadUserHeadImageData      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUploadChatImage,            m_tcpClient,&LhtTcpClient::slotUploadChatImage      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signDownloadChatImage,            m_tcpClient,&LhtTcpClient::slotDownloadChatImage      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUploadChatImageData,        m_tcpClient,&LhtTcpClient::slotUploadChatImageData      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signDownloadChatImageData,        m_tcpClient,&LhtTcpClient::slotDownloadChatImageData      ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signUpdateConnectFileServerState,m_tcpClient,&LhtTcpClient::slotUpdateConnectFileServerState,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signSendGroupMsg,         m_tcpClient,&LhtTcpClient::slotSendGroupMsg           ,Qt::QueuedConnection);


    connect(this,&GlobalCenter::signFileServer,           m_fileClient,&LhtFileClient::slotConnect2FileServer          ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signGetFileAddr,          m_fileClient,&LhtFileClient::slotSetFileBody         ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signStartRecvFile,        m_fileClient,&LhtFileClient::slotStartRecvFile           ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signStartSendFile,        m_fileClient,&LhtFileClient::slotStartSendFile           ,Qt::QueuedConnection);
    connect(this,&GlobalCenter::signSendFileData,         m_fileClient,&LhtFileClient::sendFileData           ,Qt::QueuedConnection);
    //文件进度回传显示
    connect(m_fileClient ,&LhtFileClient::signRecvFileProgress,  this,&GlobalCenter::slotRecvFileProgress          ,Qt::QueuedConnection);
    connect(m_fileClient ,&LhtFileClient::signSendFileProgress,  this,&GlobalCenter::signSendFileProgress          ,Qt::QueuedConnection);
    connect(this ,&GlobalCenter::signFileCancel,  this,&GlobalCenter::slotFileCancel          ,Qt::QueuedConnection);


    //    connect(this,&MainWindow::signSendFileRequest,      m_tcpClient,&TcpClient::sendFileRequest           ,Qt::QueuedConnection);

    connect(m_tcpClient ,&LhtTcpClient::signLoginStatus,       this,&GlobalCenter::slotLoginStatus               ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signLoginBody,         this,&GlobalCenter::slotLoginBody                 ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signRecvMsg,           this,&GlobalCenter::slotRecvMsg                   ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signOnlineUserList,    this,&GlobalCenter::slotRecvOnlineUserList        ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signOnlineUserUpdate,  this,&GlobalCenter::slotOnlineUserUpdate          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signSendFileProgress,  this,&GlobalCenter::slotSendFileProgress          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signRecvFileCompelte,  this,&GlobalCenter::slotRecvFileCompelte          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signSendFileCompelte,  this,&GlobalCenter::slotSendFileCompelte          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetOrg,            this,&GlobalCenter::slotGetOrg                    ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetUserOrg,        this,&GlobalCenter::slotGetUserOrg                ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signRecvMsgNotify,     this,&GlobalCenter::slotRecvMsgNotify          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signUpdateMsgStatus,   this,&GlobalCenter::slotUpdateMsgStatus          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetOfflineNotify,  this,&GlobalCenter::slotGetOfflineNotify          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signTpcConnected,      this,&GlobalCenter::signTpcConnected               ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signLoginStatus,       this,&GlobalCenter::signLoginStatus               ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signLoginBody,         this,&GlobalCenter::signLoginBody                 ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signRecvMsg,           this,&GlobalCenter::signRecvMsg                   ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signOnlineUserList,    this,&GlobalCenter::signOnlineUserList        ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signOnlineUserUpdate,  this,&GlobalCenter::signOnlineUserUpdate          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signSendFileProgress,  this,&GlobalCenter::signSendFileProgress          ,Qt::QueuedConnection);

    connect(m_tcpClient ,&LhtTcpClient::signRecvFileCompelte,  this,&GlobalCenter::signRecvFileCompelte          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signSendFileCompelte,  this,&GlobalCenter::signSendFileCompelte          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetOrg,            this,&GlobalCenter::signGetOrgRes                    ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetUserOrg,        this,&GlobalCenter::signGetUserOrg                ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signRecvMsgNotify,     this,&GlobalCenter::signRecvMsgNotify          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signUpdateMsgStatus,   this,&GlobalCenter::signMsgHadBeenRead          ,Qt::QueuedConnection);
    connect(m_tcpClient ,&LhtTcpClient::signGetOfflineNotify,  this,&GlobalCenter::signGetOfflineNotify          ,Qt::QueuedConnection);



    m_tcpClient->moveToThread(t1);
    m_fileClient->moveToThread(t2);
    t1->start();
    t2->start();
}
#include "Network/LhtTcpClient/Handle/handler_manager.h"

GlobalCenter::GlobalCenter(QObject *parent) : QObject(parent)
{
    HandlerManager::Instance()->init();
}

void GlobalCenter::slotLoginStatus(int status, QString str)
{
    m_loginStatus = status;
    if(status){
        SystemBody body;//登录获取组织架构
        body.UserId  = AppCache::Instance()->m_userId;
        body.SystemCMD      = "0";
        signGetOrg(body,GetOrg,0);
    }else{

    }
    signLoginStatus(status,str);
}

void GlobalCenter::slotGetUserHeadImage(int userId, QString userName)
{
    SystemBody body;//登录获取组织架构
    body.UserId  = userId;
    body.SystemCMD      = userName;
    signGetUserHeadImage(body,GetUserHeadImage,0);
}

void GlobalCenter::slotRecvMsg(MsgBody body)
{
    if(body.SendUserId != AppCache::Instance()->m_userId){
        if(body.Msg.contains(ImageEnd)){
            auto fileName = body.Msg;
            fileName.replace(ImageEnd,"");
            fileName.replace(ImageHeader,"");
            m_recvMd52msg[body.SendUserId][fileName] = body;
            auto md5 = createFileMd5(fileName);
            FileBody fbody;
            QFile file;
            file.setFileName(fileName);
            int size = file.size();
            fbody.UserId = AppCache::Instance()->m_userId;
            fbody.FileName = fileName;
            fbody.SendUserId = body.SendUserId;
            fbody.RecvUserId = body.DstUserId;
            fbody.FileMD5 = md5;
            fbody.TotalSize = size;
            fbody.CurrentSize = 0;
            fbody.FileSeek = 0;
            signDownloadChatImage(fbody);
        }else{
            signRecvMsg(body);
        }
    }else{
        signRecvMsg(body);
    }


}

void GlobalCenter::slotUpdateUserHeadImage(int userId, QString path)
{
    m_allUserIdTemp.removeOne(userId);
    signUpdateUserHeadImage( userId,  path);
    if(m_allUserIdTemp.size()>0){
        auto userId = m_allUserIdTemp.at(0);
        auto info = AppCache::Instance()->m_userInfos[userId];
        slotGetUserHeadImage(userId,info.UserLoginName);
    }
}

void GlobalCenter::slotRecvMsgNotify(MsgBody body)
{
    signRecvMsgNotify(body);
}

void GlobalCenter::slotUpdateMsgStatus(MsgBody body)
{
    signMsgHadBeenRead(body);
}

void GlobalCenter::slotGetOfflineNotify(MsgBody body)
{
    signGetOfflineNotify(body);
}

void GlobalCenter::slotRecvOnlineUserList(QString userList)
{

}

void GlobalCenter::slotOnlineUserUpdate(OnlineListBody body)
{
    signOnlineUserUpdate(body);
}

void GlobalCenter::slotRecvFileCompelte(FileBody bodyf)
{

}

void GlobalCenter::slotSendFileCompelte()
{

}

void GlobalCenter::slotCreateGroup(GroupBody body)
{
    signCreateGroup(body);
}

void GlobalCenter::slotFileCancel(FileBody body)
{
    signCloseFile(body);
    qDebug()<<"File cancel";
}

void GlobalCenter::slotLoginBody(LoginBody body)
{
    signLoginBody(body);
}

void GlobalCenter::slotGetOrg(QJsonDocument json)
{
    signGetOrgRes(json);
}

void GlobalCenter::slotGetUserOrg(QJsonDocument json)
{
    signGetUserOrg(json);
}

void GlobalCenter::slotRecvGroups(QList<GroupStruct> g)
{
    signRecvGroups(g);
}

void GlobalCenter::slotRecvGroupUsers(QList<GroupUsersStruct> g)
{
    signRecvGroupUsers(g);
}

void GlobalCenter::slotRecvFileProgress(FileBody body)
{
    signRecvFileProgress(body);
}

void GlobalCenter::slotSendFileProgress(FileBody body)
{

}

void GlobalCenter::slotFileCmd(QString path, bool type, int cmd)
{
    //取消发送或者接收
    if(cmd == 2)
    {
        FileBody body;
        body.UserId = AppCache::Instance()->m_userId;
        body.FileName = "";
        body.SendUserId = 0;
        body.RecvUserId = 0;
        body.FileMD5 = path;
        body.TotalSize = 0;
        body.CurrentSize = 0;
        body.FileSeek = 0;
        signSendFileData(FileCancelPack,body);
        //关闭文件列表
        signCloseFile(body);
        //        m_fileClient->

    }
    else
    {
        if(type == 1)
        {
            FileBody body;
            QFile file;
            file.setFileName(path);
            int size = file.size();
            body.UserId = AppCache::Instance()->m_userId;
            body.FileName = path.split("/").last();
            body.SendUserId = AppCache::Instance()->m_userId;
            body.RecvUserId = m_currentChoiseUserId;
            body.FileMD5 = createFileMd5(path);
            body.TotalSize = size;
            body.CurrentSize = 0;
            body.FileSeek = 0;
            //通知发送端去连接文件服务器
            signGetFileAddr(body,FileFirstPack,0);
        }
        else
        {
            //            signRecvFile()
        }
    }


}
void logFunction(const QString& message, QsLogging::Level level)
{

}
void GlobalCenter::initLog(QsLogging::Level level, const QString &logPath, long long maxSizeBytes, int maxOldLogCount)
{
    using namespace QsLogging;
    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(level);
    const QString sLogPath(logPath);

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                       sLogPath, EnableLogRotation, MaxSizeBytes(maxSizeBytes), MaxOldLogCount(maxOldLogCount)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));//日志执行后函数
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    logger.addDestination(functorDestination);
}

void GlobalCenter::setAllUserId(QList<int> list)
{
    m_allUserId = list;
    m_allUserIdTemp = m_allUserId;
}

void GlobalCenter::slotJitter()
{
    MsgBody body;
    // 生成一个随机的 UUID
    QUuid uuid = QUuid::createUuid();

    // 将 UUID 转换为字符串表示形式
    QString uuidString = uuid.toString();
    body.MsgId = uuidString;
    body.UserId         = AppCache::Instance()->m_userId;
    body.SendUserId     = AppCache::Instance()->m_userId;
    body.SendUserName   = AppCache::Instance()->m_userName;
    body.DstUserName    = m_currentChoiseUser;
    body.DstUserId      = m_currentChoiseUserId;
    body.Msg            = "";
    body.MsgStatus      = 1;
    body.MsgType        = 4;//抖动消息
    body.SendTime       = getCurrentTimeSeconds();
    signSendMsg(body,SendMsg,0);
    //    signJitter(body);
}

void GlobalCenter::slotSendMsg(QString md5)
{
    if(m_md52msg.find(md5) != m_md52msg.end() ){
        signSendMsg(m_md52msg[md5],SendMsg,0);
        m_md52msg.remove(md5);
    }else{
        QLOG_ERROR()<<"image msg not found body";
    }
}

void GlobalCenter::slotGetMqttServerAddr()
{
    SystemBody body;//登录获取组织架构
    body.UserId  = AppCache::Instance()->m_userId;
    body.SystemCMD      = GetMqttAddr;
    signGetMqttServerAddr(body);
}

void GlobalCenter::slotGetVideoMqttServerAddr()
{
    SystemBody body;//登录获取组织架构
    body.UserId  = AppCache::Instance()->m_userId;
    body.SystemCMD      = QString::number(GetVideoMqttAddr);
    signGetVideoMqttServerAddr(body);
}

void GlobalCenter::slotRecvMsgImage(int userId, QString fileName)
{
    if(m_recvMd52msg.find(userId) != m_recvMd52msg.end() ){
        if(m_recvMd52msg[userId].find(fileName) != m_recvMd52msg[userId].end()){
            signRecvMsg(m_recvMd52msg[userId][fileName]);
            m_recvMd52msg[userId].remove(fileName);
        }

    }else{
        QLOG_ERROR()<<"image msg not found body";
    }

}

QString GlobalCenter::getCurrentTimeSeconds()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    return current_date;
}

QString GlobalCenter::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

void GlobalCenter::openLocalDevice()
{

}

MsgBody GlobalCenter::sendMsg(QString msg, int msgType, int method)
{
    if(method == SendMsg){
        MsgBody body;
        // 生成一个随机的 UUID
        QUuid uuid = QUuid::createUuid();

        // 将 UUID 转换为字符串表示形式
        QString uuidString = uuid.toString();
        body.MsgId = uuidString;
        body.UserId         = AppCache::Instance()->m_userId;
        body.SendUserId     = AppCache::Instance()->m_userId;
        body.SendUserName   = AppCache::Instance()->m_userName;
        body.DstUserName    = m_currentChoiseUser;
        body.DstUserId      = m_currentChoiseUserId;
        body.Msg            = msg;
        body.MsgStatus      = 1;
        body.MsgType        = msgType;
        body.SendTime       = getCurrentTimeSeconds();
        if(msg.contains(ImageHeader)){//如果存在图片先发送图片
            auto fileName = msg;
            fileName.replace(ImageHeader,"");
            fileName.replace(ImageEnd,"");
            copyFileToFolder(fileName,ChatImagePath);
            auto md5 = createFileMd5(fileName);
            body.Msg = ImageHeader + md5 + "_" +fileName.split("/").last() + ImageEnd;
            m_md52msg[md5] = body;
            FileBody fbody;
            QFile file;
            file.setFileName(fileName);
            int size = file.size();
            fbody.UserId = AppCache::Instance()->m_userId;
            fbody.FileName = md5 + "_" +fileName.split("/").last();
            fbody.SendUserId = AppCache::Instance()->m_userId;
            fbody.RecvUserId = m_currentChoiseUserId;
            fbody.FileMD5 = md5;
            fbody.TotalSize = size;
            fbody.CurrentSize = 0;
            fbody.FileSeek = 0;
            signUploadChatImage(fbody);
        }else{//普通消息
            signSendMsg(body,SendMsg,0);
        }
        //发送图片

        return body;
    }

}

GroupBody GlobalCenter::sendGroupMsg(QString msg, int msgType, int method)
{
    GroupBody body;
    // 生成一个随机的 UUID
    QUuid uuid = QUuid::createUuid();

    // 将 UUID 转换为字符串表示形式
    QString uuidString = uuid.toString();
    body.MsgId = uuidString;
    body.UserId         = AppCache::Instance()->m_userId;
    body.SendUserId     = AppCache::Instance()->m_userId;
    body.SendUserName   = AppCache::Instance()->m_userName;
    body.GroupId        = m_currentChoiseGroup;
    body.GroupName      = AppCache::Instance()->m_groupInfos[m_currentChoiseGroup].GroupName;
    body.Msg            = msg;
    body.MsgType        = 0;
    body.SendTime       = getCurrentTimeSeconds();

    if(msg.contains(ImageHeader)){//如果存在图片先发送图片
        auto fileName = msg;
        fileName.replace(ImageHeader,"");
        fileName.replace(ImageEnd,"");
        copyFileToFolder(fileName,ChatImagePath);
        auto md5 = createFileMd5(fileName);
        body.Msg = ImageHeader + md5 + "_" +fileName.split("/").last() + ImageEnd;
        m_md52groupMsg[md5] = body;
        FileBody fbody;
        QFile file;
        file.setFileName(fileName);
        int size = file.size();
        fbody.UserId = AppCache::Instance()->m_userId;
        fbody.FileName = md5 + "_" +fileName.split("/").last();
        fbody.SendUserId = AppCache::Instance()->m_userId;
        fbody.RecvUserId = m_currentChoiseUserId;
        fbody.FileMD5 = md5;
        fbody.TotalSize = size;
        fbody.CurrentSize = 0;
        fbody.FileSeek = 0;
        signUploadChatImage(fbody);
    }else{//普通消息
        signSendGroupMsg(body,SendGroupMsg);
    }
    return body;
}

MsgBody GlobalCenter::sendAudioMsg(QString mqttServer,int msgType)
{
    MsgBody body;
    // 生成一个随机的 UUID
    QUuid uuid = QUuid::createUuid();

    // 将 UUID 转换为字符串表示形式
    QString uuidString = uuid.toString();
    body.MsgId = uuidString;
    body.UserId         = AppCache::Instance()->m_userId;
    body.SendUserId     = AppCache::Instance()->m_userId;
    body.SendUserName   = AppCache::Instance()->m_userName;
    body.DstUserName    = m_currentChoiseUser;
    body.DstUserId      = m_currentChoiseUserId;
    body.Msg            = mqttServer;
    body.MsgStatus      = 2;
    body.MsgType        = 3;
    body.SendTime       = getCurrentTimeSeconds();
    signSendMsg(body,msgType,0);
    return body;
}

MsgBody GlobalCenter::sendVideoMsg(QString mqttServer, int msgType)
{
    MsgBody body;
    // 生成一个随机的 UUID
    QUuid uuid = QUuid::createUuid();

    // 将 UUID 转换为字符串表示形式
    QString uuidString = uuid.toString();
    body.MsgId = uuidString;
    body.UserId         = AppCache::Instance()->m_userId;
    body.SendUserId     = AppCache::Instance()->m_userId;
    body.SendUserName   = AppCache::Instance()->m_userName;
    body.DstUserName    = m_currentChoiseUser;
    body.DstUserId      = m_currentChoiseUserId;
    body.Msg            = mqttServer;
    body.MsgStatus      = 2;
    body.MsgType        = 4;
    body.SendTime       = getCurrentTimeSeconds();
    signSendMsg(body,msgType,0);
    //打开本地摄像头和语音
    initDevice(body);
    return body;

}

void GlobalCenter::slotStartAudio(MsgBody qbody)
{
    //发送通知到界面
    signStartAudio(qbody);
}

void GlobalCenter::slotAccptAudio(MsgBody body)
{
    initAudio();
    //5.开始通话
    openAudio(body);
    //发送通知到界面
    signAccptAudio(body);
}

void GlobalCenter::slotStartVideo(MsgBody qbody)
{
    //发送通知到界面
    signStartVideo(qbody);
}

void GlobalCenter::slotAccptVideo(MsgBody body)
{
    //视频开始发送
    startCommunication();
    //发送界面通知
    signAccptVideo(body);
}

void GlobalCenter::initDevice(MsgBody body)
{
    //打开本地摄像头和语音
    initAudio();
    openAudio(body);
    initVideo();
    openVideo(body);
}

void GlobalCenter::initAudio()
{
    if(!m_audioPlay){
        QThread * th = new QThread;

        m_audioPlay = new AudioInput;
        m_audioPlay->moveToThread(th);
        th->start();

        connect(this ,&GlobalCenter::signStartRun,m_audioPlay,&AudioInput::startRead,Qt::QueuedConnection);
        connect(this ,&GlobalCenter::signStopRun,m_audioPlay,&AudioInput::stopRead,Qt::QueuedConnection);
    }

}

void GlobalCenter::initVideo()
{
    if(!m_videoPlay){
        QThread * th2 = new QThread;
        qDebug() << "yeahhh"<< av_version_info();

        m_videoPlay = new VedioPlay;

        m_videoPlay->moveToThread(th2);
        th2->start();

        connect(this,&GlobalCenter::openDevice,m_videoPlay,&VedioPlay::open,Qt::QueuedConnection);
        connect(this,&GlobalCenter::startRead,m_videoPlay,&VedioPlay::read,Qt::QueuedConnection);
        connect(this,&GlobalCenter::stopReadVideo,m_videoPlay,&VedioPlay::close,Qt::QueuedConnection);
        connect(m_videoPlay,&VedioPlay::signImageReadReady,this,&GlobalCenter::signImageReadReady,Qt::QueuedConnection);
        connect(m_videoPlay,&VedioPlay::signRemoteImageReadReady,this,&GlobalCenter::signRemoteImageReadReady,Qt::QueuedConnection);

    }
}

void GlobalCenter::openAudio(MsgBody body)
{
    int pusdId = AppCache::Instance()->m_userId;
    int recvId;
    if(body.UserId != AppCache::Instance()->m_userId){//我发出的
        recvId = body.UserId;
    }else{
        recvId = body.DstUserId;
    }
    signStartRun(body.Msg.split(":").at(0),body.Msg.split(":").at(1).toInt(), pusdId,recvId);
}

void GlobalCenter::openVideo(MsgBody body)
{
    // 获取可用摄像头列表
    QString videoName;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for(auto camera : cameras)
    {
#if defined(Q_OS_WIN)
        //        ui->com_url->addItem("video=" + camera.description());    // ffmpeg在Window下要使用video=description()
        videoName = "video=" + camera.description();
#elif defined(Q_OS_LINUX)
        ui->com_url->addItem(camera.deviceName());                // ffmpeg在linux下要使用deviceName()
#elif defined(Q_OS_MAC)
#endif
    }
    int pusdId = AppCache::Instance()->m_userId;
    int recvId;
    if(body.UserId != AppCache::Instance()->m_userId){//我发出的
        recvId = body.UserId;
    }else{
        recvId = body.DstUserId;
    }
    openDevice(videoName,body.Msg.split(":").at(0),body.Msg.split(":").at(1).toInt(), pusdId,recvId);
    //    QTimer::singleShot(1,[this]{
    startReadLocal();
    //    });
}

void GlobalCenter::startReadLocal()
{
    startRead();
}

void GlobalCenter::startCommunication()
{
    m_videoPlay->startCommunication();
}

void GlobalCenter::closeAudio()
{
    signStopRun();
}

void GlobalCenter::closeVideo()
{
    stopReadVideo();
}

void GlobalCenter::showVideoUI(MsgBody qbody)
{
    if(1){
        MsgBody body;
        // 生成一个随机的 UUID
        QUuid uuid = QUuid::createUuid();
        // 将 UUID 转换为字符串表示形式
        QString uuidString = uuid.toString();
        body.MsgId = uuidString;
        body.UserId         = AppCache::Instance()->m_userId;
        body.SendUserId     = AppCache::Instance()->m_userId;
        body.SendUserName   = AppCache::Instance()->m_userName;
        body.DstUserName    = qbody.SendUserName;
        body.DstUserId      = qbody.SendUserId;
        body.Msg            = qbody.Msg;
        body.MsgStatus      = 2;
        body.MsgType        = 4;
        body.SendTime       = getCurrentTimeSeconds();
        signSendMsg(body,NotifyVideoAccpt,0);
        initDevice(body);
        slotAccptVideo(body);
    }
}

void GlobalCenter::showAudioUI(MsgBody qbody)
{
    //3.自动同意，发送同意
    MsgBody body;
    // 生成一个随机的 UUID
    QUuid uuid = QUuid::createUuid();

    // 将 UUID 转换为字符串表示形式
    QString uuidString = uuid.toString();
    body.MsgId = uuidString;
    body.UserId         = AppCache::Instance()->m_userId;
    body.SendUserId     = AppCache::Instance()->m_userId;
    body.SendUserName   = AppCache::Instance()->m_userName;
    body.DstUserName    = qbody.SendUserName;
    body.DstUserId      = qbody.SendUserId;
    body.Msg            = qbody.Msg;
    body.MsgStatus      = 2;
    body.MsgType        = 3;
    body.SendTime       = getCurrentTimeSeconds();
    signSendMsg(body,NotifyAudioAccpt,0);
    slotAccptAudio(body);
}

void GlobalCenter::createGroup(GroupBody body)
{
    signSendGroupMsg(body,CreateGroup);
}

void GlobalCenter::slotGetGroups()
{
    GroupBody body;
    body.UserId = AppCache::Instance()->m_userId;
    signSendGroupMsg(body,GetUserGroupList);
}

void GlobalCenter::slotGetGroupUsers(QString groupId)
{
    GroupBody body;
    body.UserId = AppCache::Instance()->m_userId;
    body.GroupId = groupId;
    signSendGroupMsg(body,GetGroupUserList);
}
