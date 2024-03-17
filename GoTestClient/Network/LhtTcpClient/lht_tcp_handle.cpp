#include "lht_tcp_handle.h"
#include <QDebug>
#include <QUuid>
#include "Logic/global_center.h"

LhtTcpHandle::LhtTcpHandle(QObject *parent):QThread(parent)
{
    m_poolSize = 100;
    m_stoped = true;
    m_dataQueue.clear();
}

LhtTcpHandle::~LhtTcpHandle()
{
    m_stoped = true;
    m_waitCondition.wakeOne();
    quit();
    wait();
    m_stoped = true;
}

bool LhtTcpHandle::recvData(const QByteArray&  data)
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

void LhtTcpHandle::startRun()
{
    if(m_stoped)
        start();
}

void LhtTcpHandle::setPoolSize(unsigned size)
{
    m_poolSize = size;
}

void LhtTcpHandle::stopRun()
{
    m_stoped = true;
    m_waitCondition.wakeOne();

    quit();
    wait();
}

void LhtTcpHandle::run()
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

void LhtTcpHandle::handleData(QByteArray arr)
{
    m_buffer.append(arr);
    int size= -1;
    int method = -1;
    int methodType = -1;
    int totalLen = m_buffer.size();
    while( totalLen )
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

        std::shared_ptr<NewPacket> data(new NewPacket{Header{size,method,methodType},{m_buffer.data()+HeaderSize,size-HeaderSize}});
        arr.resize(size - HeaderSize);
        auto handle = MsgHandlerRepository<RequestHandler>::instance().handler(method);
        if(handle != nullptr)
            handle(data);
        else{
            QLOG_ERROR()<<"error cmd == "<<method;
        }

        m_buffer = m_buffer.right(totalLen - size);
        totalLen = m_buffer.size();
    }
}
