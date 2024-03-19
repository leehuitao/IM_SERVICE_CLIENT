#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QTcpServer>
#include <QTcpSocket>
#include "Logic/app_cache.h"
#include "LhtTcpFileClient/lht_file_client.h"
#include "lht_tcp_handle.h"
#include "Handle/handler_manager.h"
class LhtTcpClient: public QObject
{
    Q_OBJECT
public:
    explicit LhtTcpClient(QObject *parent = nullptr);

    void slotGetVideoMqttServerAddr(SystemBody body);
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

    void signUpdateMsgStatus(MsgBody);

    void signGetOfflineNotify(MsgBody);


    //发送到 文件线程
    void signRecvFile(FileBody);

    void signContinueSendFile(int,FileBody);

    void signSendFileProgress(FileBody);

    void signSendFileCompelte();

//    void signRecvFileProgress(int totalsize,int seek,int currentsize,int status,QString fileName);

    void signRecvFileCompelte(FileBody);

    void signGetOrg(QJsonDocument body);

    void signGetUserOrg(QJsonDocument body);

    void signSetFileSendName(QString);
public slots:
    void sendLogin(QString  ip,int port ,LoginBody body);

    void sendUpdateStatus(LoginBody body, int method ,int methodType);

    void sendLogout(LoginBody body);

    void sendMsg(MsgBody body, int method ,int methodType);

    void slotGetMqttServerAddr(SystemBody body);

    void sendGetOrg(SystemBody body, int method ,int methodType);

    void sendGetOnlineUsers(SystemBody body, int method ,int methodType);

    void sendGetUserHeadImage(SystemBody body, int method ,int methodType);

    void sendGetMsg(MsgBody body);

    void sendGetOfflineMsg(SystemBody);

    void sendUpdateMsgStatus(MsgBody);

    void slotUpdateConnectFileServerState(FileBody body);

    void slotGetFileServerAddr(FileBody body, int method ,int methodType);

    void slotUploadUserHeadImage(FileBody body);

    void slotUploadUserHeadImageData(FileBody body);

    void slotUploadChatImage(FileBody body);

    void slotDownloadChatImage(FileBody body);

    void slotUploadChatImageData(FileBody body);

    void slotDownloadChatImageData(FileBody body);

    void slotSendGroupMsg(GroupBody body,int method,int methodType = 0);
private slots:
    void init();

    void connected();

    void slotDisconnect(QAbstractSocket::SocketError socketError);

    void receiveData();

private:

private:
    QTcpSocket *            m_socket = nullptr;
    QList<MsgBody>          m_MsgCache;
    QByteArray              m_buffer;
    PacketProcess           m_packProcess;
    QString                 m_serverip;
    int                     m_serverport;
    LoginBody               m_loginBody;
    QFile   *               m_recvFile;
    FileBody                m_FileBodyCache;
    int                     m_totalSize=0;
    int                     m_currentSize=0;
    QString                 m_senderName;
    QString                 m_currentFilePath;
    FileBody                m_sendFileBody;
    FileBody                m_recvFileBody;
    bool                    fileStatus = false;//false 本机此时是发送端；true 本机此时是接收端
    //tcp解析器
    LhtTcpHandle               m_tcpHandle;
};

#endif // TCPCLIENT_H
