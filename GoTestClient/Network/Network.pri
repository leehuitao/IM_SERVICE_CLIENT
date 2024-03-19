HEADERS += \
    $$PWD/LhtTcpClient/Handle/RegisterHandler.h \
    $$PWD/LhtTcpClient/Handle/handle/connect_file_server_success_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/create_group_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/download_chat_image_data_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_file_server_addr_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_mqtt_addr_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_msg_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_msg_notify_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_offline_msg_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_online_user_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_org_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_user_groups_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_user_head_image_data_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_user_org_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/get_video_mqtt_addr_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/group_invatation_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/login_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/logout_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/meg_has_been_read_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_audio_accpt_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_audio_start_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_audio_stop_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_video_accpt_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_video_start_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/notify_video_stop_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/online_user_list_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/send_msg_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/update_connect_file_server_state_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/update_msg_status_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/update_online_user_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/upload_chat_image_data_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/upload_chat_image_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/upload_user_head_image_data_handle.h \
    $$PWD/LhtTcpClient/Handle/handle/upload_user_head_image_handle.h \
    $$PWD/LhtTcpClient/Handle/handler_common.h \
    $$PWD/LhtTcpClient/Handle/handler_manager.h \
    $$PWD/LhtTcpClient/Handle/lht_file_manager.h \
    $$PWD/LhtTcpClient/Handle/msghandler_repository.h \
    $$PWD/LhtTcpClient/lht_tcp_client.h \
    $$PWD/LhtTcpClient/lht_tcp_handle.h \
    $$PWD/LhtTcpFileClient/lht_file_client.h \
    $$PWD/LhtTcpFileClient/lht_file_handle.h \
    $$PWD/packet_define.h \
    $$PWD/packet_process.h

SOURCES += \
    $$PWD/LhtTcpClient/Handle/RegisterHandler.cpp \
    $$PWD/LhtTcpClient/Handle/handler_manager.cpp \
    $$PWD/LhtTcpClient/Handle/lht_file_manager.cpp \
    $$PWD/LhtTcpClient/lht_tcp_client.cpp \
    $$PWD/LhtTcpClient/lht_tcp_handle.cpp \
    $$PWD/LhtTcpFileClient/lht_file_client.cpp \
    $$PWD/LhtTcpFileClient/lht_file_handle.cpp \
    $$PWD/packet_process.cpp

INCLUDEPATH += $$PWD
