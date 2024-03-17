#ifndef UPLOAD_CHAT_IMAGE_DATA_HANDLE_H
#define UPLOAD_CHAT_IMAGE_DATA_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UploadChatImageDataHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UploadChatImageDataHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    if(body.FilePause == 1){
        GlobalCenter::getInstance()->slotSendMsg(body.FileMD5);
    }else{
        QFile file;
        auto fname = body.FileName;
        file.setFileName(ChatImagePath + fname);
        if(file.open(QIODevice::ReadOnly)){
            file.seek(body.FileSeek);
            auto data = file.read(8192);
            body.CurrentSize = data.size();
            body.FileData = data;
            GlobalCenter::getInstance()->signUploadChatImageData(body);

        }else{
            body.FilePause = 1;
            body.FileData.clear();
            GlobalCenter::getInstance()->signUploadChatImageData(body);
        }
        file.close();
    }

}



#endif // LOGIN_HANDLE_H
