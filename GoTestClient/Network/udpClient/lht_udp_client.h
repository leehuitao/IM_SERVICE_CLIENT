#ifndef LHT_UDP_CLIENT_H
#define LHT_UDP_CLIENT_H

#include "utils/thread_pool_basic.h"
#include "utils/lht_callback.h"
#include "utils/lht_define.h"
#include <QUdpSocket>

class LhtUdpClient : public QObject
        ,public LhtCallback
{
    Q_OBJECT
public:
    LhtUdpClient();

public slots:
    void init(QString ip , int port,QString tIp ,int tPort);

    void onReadyRead();
    // LhtCallback interface
    bool lhtReadyRead(QByteArray arr) override;

    void setCallback(LhtCallback * c){
        m_lhtCallback = c;
    }
protected:


private:
    QUdpSocket * m_udpClient;

    QString m_ip;

    int m_port;

    LhtCallback * m_lhtCallback = nullptr;

    QByteArray m_cache;

    QHostAddress m_targetAddress; // 目标 IP 地址

    quint16 m_targetPort; // 目标端口

    char m_readData[1024];
};

#endif // LHT_UDP_CLIENT_H
