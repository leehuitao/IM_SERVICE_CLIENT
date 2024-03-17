#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include "packet_process.h"
class LhtFileHandle:public QThread
{
    Q_OBJECT
public:
    LhtFileHandle();

    ~LhtFileHandle();

    bool recvData(QByteArray data);

    void startRun();

    void setPoolSize(unsigned size);

    void stopRun();

    void setFileBody(FileBody body);

    void setFile(QFile*file)
    {
        m_file = file;
    }
signals:
    void signRecvFileProgress(FileBody body);

    // QThread interface
protected:
    void run();

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

    FileBody                m_recvFileBody;

    FileBody                m_sendFileBody;

    bool                    fileStatus;

    QFile                   *m_file;

    QByteArray              m_buffer;
};

#endif // FILEHANDLE_H
