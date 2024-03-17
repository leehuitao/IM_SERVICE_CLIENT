#ifndef LHT_DEFINE_H
#define LHT_DEFINE_H
#include <QObject>
#include <QDebug>

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

enum LhtCodeType {
    LhtEncode,
    LhtDecode
};

struct UdpHeader{
    char ecode1 = 0x76;
    char ecode2 = 0x67;
    unsigned short packSize;
    unsigned short cmdId;
};

struct UdpPack{
    UdpPack(QByteArray d ,int type = 0){
        if(type == 0){
            data = d;
            header.packSize = d.size() + sizeof (UdpHeader);
            header.cmdId = 0;
        }else{
            header.packSize |= (unsigned char)d.at(2) ;
            header.packSize |= (unsigned char)d.at(3) << 8 ;
            header.cmdId |= (unsigned char)d.at(4) ;
            header.cmdId |= (unsigned char)d.at(5) << 8 ;
//            qDebug()<<"sizeof (UdpHeader)"<<sizeof (UdpHeader);
            data = d.mid(sizeof (UdpHeader),header.packSize - sizeof (UdpHeader));
        }
    }

    QByteArray toArr(){
        QByteArray arr;
        arr.append(this->header.ecode1);
        arr.append(this->header.ecode2);
        arr.append((const char *)&this->header.packSize , 2);
        arr.append((const char *)&this->header.cmdId ,2);
        arr.append(data);
//        qDebug()<<"arr size = "<<arr.size();
        return arr;
    }
    UdpHeader header;
    QByteArray data;
};

#endif // LHT_DEFINE_H
