#ifndef UPLOAD_CHAT_IMAGE_HANDLE_H
#define UPLOAD_CHAT_IMAGE_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UploadChatImageHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UploadChatImageHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    QFile file;
    auto fname = body.FileName;
    file.setFileName(ChatImagePath + fname);
    if(file.open(QIODevice::ReadOnly)){
        file.seek(0);
        auto data = file.read(8192);
        body.CurrentSize = data.size();
        body.FileData = data;
        GlobalCenter::getInstance()->signUploadChatImageData(body);
    }else{
        QLOG_ERROR()<<"open head image error path = "<<AppCache::Instance()->m_userHeadImagePath<<file.errorString();
    }
    file.close();
}



#endif // LOGIN_HANDLE_H
