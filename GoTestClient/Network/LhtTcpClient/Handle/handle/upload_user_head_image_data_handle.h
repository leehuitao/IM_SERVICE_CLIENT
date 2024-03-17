#ifndef UPLOAD_USER_HEAD_IMAGE_DATA_HANDLE_H
#define UPLOAD_USER_HEAD_IMAGE_DATA_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class UploadUserHeadImageDataHandle
{
public:
    virtual void HandleMessage(NewTcpProtocolPtr msg);

};

void UploadUserHeadImageDataHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseFileDataPack(msg.get()->data);
    if(body.FilePause == 1){
        GlobalCenter::getInstance()->signUploadHeadImageState(body.FileSeek == body.TotalSize);
        //设置头像路径
        AppCache::Instance()->m_headImagePath[AppCache::Instance()->m_userId] = AppCache::Instance()->m_userHeadImagePath;
        //更新头像
        GlobalCenter::getInstance()->slotUpdateUserHeadImage(AppCache::Instance()->m_userId,AppCache::Instance()->m_userHeadImagePath);
    }else{
        auto headFile = LhtFileManager::getFilePtr(AppCache::Instance()->m_userId,AppCache::Instance()->m_userHeadImagePath);
        if(headFile->isOpen()){
            headFile->seek(body.FileSeek);
            auto data = headFile->read(8192);
            body.CurrentSize = data.size();
            body.FileData = data;
            GlobalCenter::getInstance()->signUploadUserHeadImageData(body);

        }else{
            body.FilePause = 1;
            body.FileData.clear();
            GlobalCenter::getInstance()->signUploadUserHeadImageData(body);
        }
    }
}



#endif // LOGIN_HANDLE_H
