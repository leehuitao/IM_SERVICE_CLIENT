#ifndef GET_USER_HEAD_IMAGE_DATA_HANDLE_H
#define GET_USER_HEAD_IMAGE_DATA_HANDLE_H
#include <QByteArray>
#include <QObject>
#include "../../../packet_define.h"
#include "../handler_common.h"
#include "Logic/global_center.h"
#include "packet_process.h"
class GetUserHeadImageDataHandle
{
public:
     void HandleMessage(NewTcpProtocolPtr msg);

};

void GetUserHeadImageDataHandle::HandleMessage(NewTcpProtocolPtr msg)
{
    if (!msg)
    {
        return;
    }
    FileBody body;
    body = PacketProcess::parseHeadImagePack(msg.get()->data,msg.get()->header.MethodType);
    LhtFileManager::openHeadFile(body.UserId,body.FileName);
    auto file = LhtFileManager::getFilePtr(body.UserId,body.FileName);

    file->seek(body.FileSeek);
    file->write(msg.get()->data.mid(msg.get()->header.MethodType,body.CurrentSize));
    if(file->size() >= body.TotalSize){
        file->close();
        //设置头像路径
        AppCache::Instance()->m_headImagePath[body.UserId] = "./" + body.FileName;
        //更新头像
        GlobalCenter::getInstance()->slotUpdateUserHeadImage(body.UserId,"./" + body.FileName);
    }
}



#endif // LOGIN_HANDLE_H
