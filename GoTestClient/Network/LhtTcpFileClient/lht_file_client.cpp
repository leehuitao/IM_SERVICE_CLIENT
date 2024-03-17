#include "lht_file_client.h"
#include <QHostAddress>
#include "../Logic/global_center.h"

//#define FileBufferSize 1024*8

LhtFileClient::LhtFileClient(QObject *parent) : QObject(parent)
{
    //    m_file1.setFileName("./niu.txt");
    //    m_file1.open(QIODevice::WriteOnly);
    connect(&m_fileHandle,&LhtFileHandle::signRecvFileProgress,this,&LhtFileClient::signRecvFileProgress);

}
//连接文件服务器
void LhtFileClient::slotConnect2FileServer(QString ip, int port)
{
    m_serverip = ip;
    m_serverport = port;

    if(m_socket != nullptr){
        disconnect(m_socket,&QTcpSocket::connected,this,&LhtFileClient::connected);
        disconnect(m_socket,&QTcpSocket::readyRead,this,&LhtFileClient::receiveData);
        m_socket->close();
        m_socket = nullptr;
    }
    m_socket = new QTcpSocket;
    m_socket->connectToHost(QHostAddress(ip),port);
    connect(m_socket,&QTcpSocket::connected,this,&LhtFileClient::connected);
    connect(m_socket,&QTcpSocket::readyRead,this,&LhtFileClient::receiveData);
}

void LhtFileClient::slotSetFileBody(FileBody body, int method, int methodType)
{
    m_currentFileBodyCache = body;
}



void LhtFileClient::connected()
{
    //    qDebug()<<"文件服务器连接成功";
    m_fileHandle.startRun();
    m_currentFileBodyCache.UserId = AppCache::Instance()->m_userId;
    //暂时这么写，连接上了文件服务器
    if(AppCache::Instance()->m_userId == m_currentFileBodyCache.SendUserId)
    {
        sendFileData(FileFirstPack,m_currentFileBodyCache);
    }
    else
    {
        GlobalCenter::getInstance()->signUpdateConnectFileServerState(m_currentFileBodyCache);
    }
}

void LhtFileClient::receiveData()
{
    //缓冲区没有数据，直接无视
    if( m_socket->bytesAvailable() <= 0 )
    {
        return;
    }

    QByteArray buffer;
    buffer = m_socket->readAll();

    m_fileHandle.recvData(buffer);
}

void LhtFileClient::sendFileData(int method, FileBody body)
{
    if(method == FileFirstPack)
    {
        body.UserId = AppCache::Instance()->m_userId;
        //启动发送文件第一个包
        Pack pack(body,method,0);
        auto data = pack.toByte();
        m_socket->write(data);
        if(body.SendUserId == AppCache::Instance()->m_userId)
        {
            //打开文件
            m_file = new QFile(AppCache::Instance()->m_currentFilePath);
            m_file->open(QFile::ReadOnly);
        }

    }
    else if (method == UpdateConnectFileServerState)
    {
        Pack pack(body,method,0);
        auto data = pack.toByte();
        m_socket->write(data);
    }
    else if (method == FileDataPack)
    {
        m_mutex.lock();
        body.UserId = AppCache::Instance()->m_userId;
        if(body.FileSeek < body.TotalSize)
        {
            m_file->seek(body.FileSeek);
            body.FileData = m_file->read(8192);
            body.CurrentSize = body.FileData.size();
            //            m_file1.write(body.FileData,body.CurrentSize);
            Pack pack(body,method,0);
            auto data = pack.toByte();
            m_socket->write(data);
        }
        signSendFileProgress(body);
        m_mutex.unlock();
    }else if(method == FileCancelPack){
        Pack pack(body,method,0);
        auto data = pack.toByte();
        m_socket->write(data);
        m_socket->close();
    }
    else {
        Pack pack(body,method,0);
        auto data = pack.toByte();
        m_socket->write(data);
    }
}

void LhtFileClient::slotCancel(QString md5)
{
    FileBody body ;
    body.UserId = AppCache::Instance()->m_userId;
    Pack pack(body,FileCancelPack,0);
    auto data = pack.toByte();
    m_socket->write(data);
}


//接收端准备开始接收文件 FileDataPack
void LhtFileClient::slotStartRecvFile(FileBody body)
{
    m_recvFile = new QFile(body.FileName);
    bool status = m_recvFile->open(QFile::WriteOnly|QFile::Append|QFile::Truncate);
    if(status)
    {
        qDebug()<<"file start recv "<<body.FileName;
        //接收端发送FileFirstPack包，然后准备开始接收文件
        body.UserId = AppCache::Instance()->m_userId;
        sendFileData(FileFirstPack,body);
        //signRecvFileProgress(m_totalSize,0,1,m_currentFileName);
        m_fileHandle.setFile(m_recvFile);
    }
    else
    {
        qDebug()<<"file start recv error "<<body.FileName;
    }
}
//发送端准备开始发送文件 FileDataPack
void LhtFileClient::slotStartSendFile(FileBody body)
{
    body.UserId = AppCache::Instance()->m_userId;
    sendFileData(FileDataPack,body);
}


