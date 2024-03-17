#ifndef DOWNLOAD_CHAT_IMAGE_DATA_HANDLE_H
#define DOWNLOAD_CHAT_IMAGE_DATA_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class DownloadChatImageDataHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void DownloadChatImageDataHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data,msg.get()->header.MethodType);

    if(body.FilePause == 1){

        QFile file;
        file.setFileName(ChatImagePath + body.FileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
            file.seek(body.FileSeek);
            file.write(body.FileData);
            body.FileSeek = body.FileSeek + body.CurrentSize;
            body.FileData.clear();
        }
        file.close();
        GlobalCenter::getInstance()->slotRecvMsgImage(body.SendUserId,body.FileName);

    }else{
        QFile file;
        file.setFileName(ChatImagePath + body.FileName);
        if(file.open(QIODevice::WriteOnly| QIODevice::Append)){
            file.seek(body.FileSeek);
            file.write(body.FileData);
            body.FileSeek = body.FileSeek + body.CurrentSize;
            body.FileData.clear();
            GlobalCenter::getInstance()->signDownloadChatImageData(body);

        }else{
            body.FilePause = 1;
            body.FileData.clear();
            GlobalCenter::getInstance()->signDownloadChatImageData(body);
        }
        file.close();
    }
}



#endif // LOGIN_HANDLE_H
