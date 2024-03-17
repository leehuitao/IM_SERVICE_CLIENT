#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "packet_define.h"
#include "packet_process.h"
#include "Logic/app_cache.h"
#include "lht_file_handle.h"
class LhtFileClient : public QObject
{
    Q_OBJECT
public:
    explicit LhtFileClient(QObject *parent = nullptr);
    void init(QString  ip,int port);

    void setFilePath(QString path);
public slots:
    void slotConnect2FileServer(QString ip,int port);

    void slotSetFileBody(FileBody body, int method, int methodType);

    void slotStartRecvFile(FileBody body);

    void slotStartSendFile(FileBody body);

    void sendFileData(int method,FileBody body);

    void slotCancel(QString md5);
private:
    void connected();

    void receiveData();
signals:
    void signContinueSendFile(int,FileBody);


    //文件发送完成
    void signSendFileCompelte();

    void signUpdateFileServer(FileBody);
    //文件接收进度
    void signRecvFileProgress(FileBody body);
    //文件发送进度
    void signSendFileProgress(FileBody body);
    //文件取消发送
    void signFileCancel(FileBody body);
private:
    QMutex              m_mutex;
    QTcpSocket *        m_socket = nullptr;
    QByteArray          m_buffer;
    PacketProcess       m_packProcess;
    QString             m_serverip;
    int                 m_serverport;
    QFile*              m_recvFile;
    int                 m_senderId;
    QString             m_currentFileName;
    FileBody            m_currentFileBodyCache;
    QFile*              m_file;
    int                 m_totalSize;
    int                 m_currentSize;
    LhtFileHandle       m_fileHandle;
    QFile               m_file1;
};

#endif // FILECLIENT_H
