#include "lht_file_handle.h"
#include <QDebug>
#include "Logic/global_center.h"

LhtFileHandle::LhtFileHandle()
{
    m_poolSize = 100;
    m_stoped = true;
    m_dataQueue.clear();
}

LhtFileHandle::~LhtFileHandle()
{
    m_stoped = true;
    m_waitCondition.wakeOne();
    quit();
    wait();
    m_stoped = true;
}

bool LhtFileHandle::recvData(QByteArray data)
{
    QMutexLocker lock(&m_dataMutex);
    if(static_cast<unsigned>(m_dataQueue.size()) > m_poolSize){
        QLOG_ERROR()<<"Queue out of size ";
        return false;
    }else{
        m_dataQueue.append(data);
        m_waitCondition.wakeOne();
        return true;
    }
}

void LhtFileHandle::startRun()
{
    if(m_stoped)
        start();
}

void LhtFileHandle::setPoolSize(unsigned size)
{
    m_poolSize = size;
}

void LhtFileHandle::stopRun()
{
    m_stoped = true;
    m_waitCondition.wakeOne();

    quit();
    wait();
}

void LhtFileHandle::setFileBody(FileBody body)
{
    m_sendFileBody = body;
}

void LhtFileHandle::run()
{
    m_dataQueue.clear();
    m_stoped = false;

    while(1){
        QMutexLocker lock(&m_dataMutex);
        if(m_dataQueue.isEmpty())
            m_waitCondition.wait(&m_dataMutex);
        else{
            auto data = m_dataQueue.takeFirst();
            lock.unlock();
            handleData(data);
        }

        if(m_stoped){
            break;
        }
    }
}

void LhtFileHandle::handleData(QByteArray arr)
{
    m_buffer.append(arr);

    int size= -1;
    int method = -1;
    int methodType = -1;
    int totalLen = m_buffer.size();
    while (totalLen)
    {
        QDataStream packet(m_buffer);

        if( totalLen < HeaderSize )//包头长度
        {
            break;
        }
        packet >> size >> method >> methodType;

        if( totalLen < size )
        {
            break;
        }
        char *rbytes = new char[size - HeaderSize];
        memset(rbytes,0,size-HeaderSize);
        memcpy(rbytes,m_buffer.data()+HeaderSize,size-HeaderSize);
        QByteArray arr(rbytes,size-HeaderSize);
        arr.resize(size - HeaderSize);
        if(method == FileFirstPack)
        {
            qDebug()<<"start send file";
            FileBody body;
            body = m_packProcess.parseFileDataPack(arr);
            if(body.SendUserId == AppCache::Instance()->m_userId)
                GlobalCenter::getInstance()->signUpdateConnectFileServerState(body);
        }
        else if(method == FileDataPack)
        {
            FileBody body;
            body = m_packProcess.parseFileDataPack(arr,methodType);
            m_file->seek(body.FileSeek);
            m_file->write(arr.mid(methodType,body.CurrentSize));
            signRecvFileProgress(body);
            if(m_file->size() >= body.TotalSize){
                m_file->close();
            }

        }
        else if (method == FileDataResPack)
        {
            FileBody body;
            body = m_packProcess.parseFileDataPack(arr);
            GlobalCenter::getInstance()->signSendFileData(FileDataPack,body);
        }
        else if (method == FileCancelPack)
        {
            FileBody body;
            body = m_packProcess.parseFileDataPack(arr);
            GlobalCenter::getInstance()->signFileCancel(body);
        }
        delete [] rbytes;
        m_buffer = m_buffer.right(totalLen - size);
        totalLen = m_buffer.size();
    }
}
















