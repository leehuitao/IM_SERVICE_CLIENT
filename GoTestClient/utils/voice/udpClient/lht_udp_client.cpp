#include "lht_udp_client.h"

LhtUdpClient::LhtUdpClient()
{

}

void LhtUdpClient::init(QString ip, int port, QString tIp, int tPort)
{
    m_ip = ip;
    m_port = port;
    m_targetAddress = QHostAddress(tIp);
    m_targetPort = tPort;
    if(m_ip.isEmpty()){
        qDebug()<<"ip or port is empty please check";
        return;
    }

    QHostAddress address(m_ip);
    m_udpClient = new QUdpSocket(this);
    if (!m_udpClient->bind(address, port)) {
        qDebug() << "bind faild" << m_udpClient->errorString();
    } else {
        qDebug() << "bind success";
    }
    connect(m_udpClient, &QUdpSocket::readyRead, this, &LhtUdpClient::onReadyRead);

}

void LhtUdpClient::onReadyRead()
{
    QByteArray arr;
    arr.resize(1024);
    auto len = m_udpClient->readDatagram(arr.data(),1024);
    if(len > 0){
        m_cache.append(arr.data(),len);
        qDebug()<<"m_cache size = "<<m_cache.size();
        while(m_cache.size() >= sizeof (UdpHeader)){
            if(m_cache.at(0)==0x76 && m_cache.at(1)==0x67){
                unsigned short size =0;
                size |= (unsigned char)m_cache.at(2) ;
                size |= (unsigned char)m_cache.at(3) << 8 ;
                if(m_cache.size() >= m_cache.at(2)){
                    if(m_lhtCallback){
                        m_lhtCallback->lhtReadyRead(m_cache.mid(0,size));
                    }
                    m_cache = m_cache.remove(0,size);
                    qDebug()<<"remove size = "<<m_cache.size();
                }
            }else{
                int index = 0;
                while(1){
                    if(index < m_cache.size() - 4){
                        if(m_cache.at(index) == 0x76 && m_cache.at(index + 1)==0x67){
                            m_cache.remove(0,index);
                            break;
                        }
                    }else{
                        m_cache.clear();
                        break;
                    }

                }
            }

//            arr.remove(0,size);
        }
    }

}

//需要发送时这里会有数据
bool LhtUdpClient::lhtReadyRead(const QByteArray &arr)
{
    UdpPack pack(arr);
    m_udpClient->writeDatagram((const char *)pack.toArr(),(int)pack.header.packSize, m_targetAddress, m_targetPort);
    return 1;
}
