#ifndef TCPHANDLE_H
#define TCPHANDLE_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include "packet_process.h"
class LhtTcpHandle :public QThread
{
    Q_OBJECT
public:
    explicit LhtTcpHandle(QObject *parent = nullptr);

    ~LhtTcpHandle();

    bool recvData(const QByteArray& data);

    void startRun();

    void setPoolSize(unsigned size);

    void stopRun();

signals:
protected:

    void run() override;
    //子类重新实现一下自己需要的处理方法
    void handleData(QByteArray arr);
private:
    //线程锁
    QMutex                  m_dataMutex;
    //无数据时暂停线程，新数据进入时唤醒线程
    QWaitCondition          m_waitCondition;
    //数据队列最大长度
    unsigned                m_poolSize;
    //数据队列
    QQueue<QByteArray>      m_dataQueue;

    volatile bool           m_stoped;
    // 0表示解析 1表示数据库
    int                     m_type;

    int                     m_parseType;

    PacketProcess           m_packProcess;

    QByteArray              m_buffer;

    QFile                   m_file;

    QMap<int,QFile>         m_chatFile;

    QFile                   m_headFile;

//    FileClient *            m_fileClient = nullptr;
};

#endif // TCPHANDLE_H
