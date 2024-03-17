#ifndef UPLOAD_USER_HEAD_IMAGE_HANDLE_H
#define UPLOAD_USER_HEAD_IMAGE_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UploadUserHeadImageHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UploadUserHeadImageHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    auto headFile = LhtFileManager::openReadFile(AppCache::Instance()->m_userId,AppCache::Instance()->m_userHeadImagePath);
//    m_headFile.setFileName(AppCache::Instance()->m_userHeadImagePath);
    if(headFile != nullptr){
        headFile->seek(0);
        auto data = headFile->read(8192);
        body.CurrentSize = data.size();
        body.FileData = data;
        GlobalCenter::getInstance()->signUploadUserHeadImageData(body);
    }else{
        QLOG_ERROR()<<"open head image error path = "<<AppCache::Instance()->m_userHeadImagePath<<headFile->errorString();
    }
}



#endif // LOGIN_HANDLE_H
