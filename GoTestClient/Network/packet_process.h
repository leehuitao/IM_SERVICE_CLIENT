#ifndef PACKETPROCESS_H
#define PACKETPROCESS_H

#include "packet_define.h"

class PacketProcess
{
public:
    PacketProcess();

    static LoginBody       parseLoginPack(QByteArray arr);

    static MsgBody         parseMsgPack(QByteArray arr);

    static OnlineListBody  parseOnlineListBodyPack(QByteArray arr);

    static SystemBody      parseSystemPack(QByteArray arr,int jsonSize = 0);

    static FileBody        parseFileDataPack(QByteArray arr,int jsonSize = 0);

    static FileBody        parseHeadImagePack(QByteArray arr,int jsonSize = 0);
};

#endif // PACKETPROCESS_H
