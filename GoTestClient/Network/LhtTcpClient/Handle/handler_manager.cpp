#include "handler_manager.h"
#include "handler_common.h"

#include "handle/login_handle.h"
#include "handle/logout_handle.h"
#include "handle/connect_file_server_success_handle.h"
#include "handle/download_chat_image_data_handle.h"
#include "handle/get_file_server_addr_handle.h"
#include "handle/get_mqtt_addr_handle.h"
#include "handle/get_msg_handle.h"
#include "handle/get_msg_notify_handle.h"
#include "handle/get_offline_msg_handle.h"
#include "handle/get_online_user_handle.h"
#include "handle/get_org_handle.h"
#include "handle/get_user_head_image_data_handle.h"
#include "handle/get_user_org_handle.h"
#include "handle/get_video_mqtt_addr_handle.h"
#include "handle/meg_has_been_read_handle.h"
#include "handle/notify_audio_accpt_handle.h"
#include "handle/notify_audio_start_handle.h"
#include "handle/notify_audio_stop_handle.h"
#include "handle/notify_video_accpt_handle.h"
#include "handle/notify_video_start_handle.h"
#include "handle/notify_video_stop_handle.h"
#include "handle/online_user_list_handle.h"
#include "handle/send_msg_handle.h"
#include "handle/update_connect_file_server_state_handle.h"
#include "handle/update_msg_status_handle.h"
#include "handle/update_online_user_handle.h"
#include "handle/upload_chat_image_data_handle.h"
#include "handle/upload_chat_image_data_handle.h"
#include "handle/upload_chat_image_handle.h"
#include "handle/upload_user_head_image_data_handle.h"
#include "handle/upload_user_head_image_handle.h"
#include "handle/group_invatation_handle.h"
#include "handle/create_group_handle.h"
#include "handle/get_user_groups_handle.h"
HandlerManager * HandlerManager::g_instance = new HandlerManager;

HandlerManager::HandlerManager(QObject *parent) : QObject(parent)
{

}

template<class Handler>
bool HandlerManager::InitHandler(const unsigned short cmd)
{
    RegisterHandler::Instance()->InitHandler(cmd, std::bind(&HandlerManager::NotifyHandler<Handler>, this, std::placeholders::_1));
    return 1;
}

template<class Handler>
void HandlerManager::NotifyHandler(NewTcpProtocolPtr msg)
{
    Handler handler;
    handler.HandleMessage(msg);
}


HandlerManager *HandlerManager::Instance()
{
    return g_instance;
}

void HandlerManager::init()
{
    //初始化接收方法
    initRecvMethod();
    //初始化返回包统一接口
    initResponseMethod();
    initVideoMethod();
    initMsgMethod();
    //初始化文件接收
    initGroupMethod();
    //初始化文件发送
    initSendFileMethod();
}

void HandlerManager::initRecvMethod()
{
    InitHandler<LoginHandle>(Login);
    InitHandler<LogoutHandle>(Logout);
    InitHandler<ConnectFileServerSuccessHandle>(ConnectFileServerSuccess);
    InitHandler<DownloadChatImageDataHandle>(DownloadChatImageData);
    InitHandler<GetFileServerAddrHandle>(GetFileServerAddr);
    InitHandler<GetOnlineUserHandle>(GetOnlineUser);
    InitHandler<GetOrgHandle>(GetOrg);
    InitHandler<GetUserHeadImageDataHandle>(GetUserHeadImageData);
    InitHandler<GetUserOrgHandle>(GetUserOrg);
    InitHandler<OnlineUserListHandle>(OnlineUserList);
    InitHandler<UpdateConnectFileServerStateHandle>(UpdateConnectFileServerState);
    InitHandler<UpdateOnlineUserHandle>(UpdateOnlineUser);
    InitHandler<UploadUserHeadImageDataHandle>(UploadUserHeadImageData);
    InitHandler<UploadUserHeadImageHandle>(UploadUserHeadImage);
}

void HandlerManager::initResponseMethod()
{

}

void HandlerManager::initVideoMethod()
{
    InitHandler<GetVideoMqttAddrHandle>(GetVideoMqttAddr);
    InitHandler<NotifyAudioAccptHandle>(NotifyAudioAccpt);
    InitHandler<NotifyAudioStartHandle>(NotifyAudioStart);
    InitHandler<NotifyAudioStopHandle>(NotifyAudioStop);
    InitHandler<NotifyVideoAccptHandle>(NotifyVideoAccpt);
    InitHandler<NotifyVideoStartHandle>(NotifyVideoStart);
    InitHandler<NotifyVideoStopHandle>(NotifyVideoStop);
    InitHandler<GetMqttAddrHandle>(GetMqttAddr);
}

void HandlerManager::initMsgMethod()
{
    InitHandler<GetMsgHandle>(GetMsg);
    InitHandler<GetMsgNotifyHandle>(GetMsgNotify);
    InitHandler<GetOfflineMsgHandle>(GetOfflineMsg);
    InitHandler<MsgHasBeenReadHandle>(MegHasBeenRead);
    InitHandler<SendMsgHandle>(SendMsg);
    InitHandler<UploadChatImageDataHandle>(UploadChatImageData);
    InitHandler<UploadChatImageHandle>(UploadChatImage);
    InitHandler<UpdateMsgStatusHandle>(UpdateMsgStatus);
}

void HandlerManager::initGroupMethod()
{
    InitHandler<CreateGroupHandle>(CreateGroup);
    InitHandler<GroupInvitationHandle>(GroupInviteMember);
    InitHandler<GetUserGroupsHandle>(GetUserGroupList);
}

void HandlerManager::initSendFileMethod()
{
}

