#include "lht_tcp_client.h"

LhtTcpClient::LhtTcpClient(QObject *parent)
{
    init();
}

void LhtTcpClient::sendLogin(QString ip, int port, LoginBody body)
{
    m_serverip = ip;
    m_serverport = port;
    m_loginBody = body;
    m_tcpHandle.startRun();
    if(m_socket != nullptr){
        disconnect(m_socket,&QTcpSocket::connected,this,&LhtTcpClient::connected);
        disconnect(m_socket,&QTcpSocket::readyRead,this,&LhtTcpClient::receiveData);
        disconnect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &LhtTcpClient::slotDisconnect);
        m_socket->close();
        m_socket = nullptr;
    }
    m_socket = new QTcpSocket;
    m_socket->connectToHost(QHostAddress(ip),port);
    connect(m_socket,&QTcpSocket::connected,this,&LhtTcpClient::connected);
    connect(m_socket,&QTcpSocket::readyRead,this,&LhtTcpClient::receiveData);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &LhtTcpClient::slotDisconnect);

}

void LhtTcpClient::sendUpdateStatus(LoginBody body, int method, int methodType)
{
    Pack pack(body,method,methodType);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendLogout(LoginBody body)
{
    Pack pack(body,Logout,0);
    auto data = pack.toByte();
    qDebug()<<"logout data:"<<data;
    m_socket->write(data);
}

void LhtTcpClient::sendMsg(MsgBody body, int method, int methodType)
{
    Pack pack(body,method,methodType);
    auto data = pack.toByte();
    qDebug()<<"data"<<data.size();
    m_socket->write(data);
}

void LhtTcpClient::slotGetMqttServerAddr(SystemBody body)
{
    Pack pack(body,GetMqttAddr,0);
    auto data = pack.toByte();
    qDebug()<<"data"<<data.size();
    m_socket->write(data);
}
void LhtTcpClient::slotGetVideoMqttServerAddr(SystemBody body)
{
    Pack pack(body,GetVideoMqttAddr,0);
    auto data = pack.toByte();
    qDebug()<<"data"<<data.size();
    m_socket->write(data);
}

//获取文件服务器的ip端口
void LhtTcpClient::slotGetFileServerAddr(FileBody body, int method ,int methodType)
{
    SystemBody sysbody;
    sysbody.UserId = AppCache::Instance()->m_userId;
    Pack pack(sysbody,GetFileServerAddr,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotUploadUserHeadImage(FileBody body)
{
    Pack pack(body,UploadUserHeadImage,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotUploadUserHeadImageData(FileBody body)
{
    Pack pack(body,UploadUserHeadImageData,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotUploadChatImage(FileBody body)
{
    Pack pack(body,UploadChatImage,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotDownloadChatImage(FileBody body)
{
    Pack pack(body,DownloadChatImage,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotUploadChatImageData(FileBody body)
{
    Pack pack(body,UploadChatImageData,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotDownloadChatImageData(FileBody body)
{
    Pack pack(body,DownloadChatImageData,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotSendGroupMsg(GroupBody body, int method, int methodType)
{
    Pack pack(body,method,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendUpdateGroupMsgStatus(GroupBody body)
{
    Pack pack(body,UpdateGroupMsgState,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendGetOrg(SystemBody body, int method, int methodType)
{
    Pack pack(body,method,methodType);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendGetOnlineUsers(SystemBody body, int method, int methodType)
{
    Pack pack(body,method,methodType);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendGetUserHeadImage(SystemBody body, int method, int methodType)
{
    Pack pack(body,method,methodType);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendGetMsg(MsgBody body)
{
    Pack pack(body,GetMsg,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendGetOfflineMsg(SystemBody body)
{
    Pack pack(body,GetOfflineMsg,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::sendUpdateMsgStatus(MsgBody body)
{
    Pack pack(body,UpdateMsgStatus,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::init()
{
    HandlerManager::Instance()->init();
}

void LhtTcpClient::connected()
{
    qDebug()<<"tcp connected";
    signTpcConnected();
    Pack pack(m_loginBody,Login,0);
    auto data = pack.toByte();
    m_socket->write(data);
}

void LhtTcpClient::slotDisconnect(QAbstractSocket::SocketError socketError)
{
    signLoginStatus(0,"连接断开");
}

void LhtTcpClient::receiveData()
{
    //缓冲区没有数据，直接无视
    if( m_socket->bytesAvailable() <= 0 )
    {
        return;
    }

    QByteArray buffer;
    buffer = m_socket->readAll();

    m_tcpHandle.recvData(buffer);
}

void LhtTcpClient::slotUpdateConnectFileServerState(FileBody body)
{
    body.UserId = AppCache::Instance()->m_userId;
    Pack pack(body,UpdateConnectFileServerState,0);
    auto data = pack.toByte();
    m_socket->write(data);
}
