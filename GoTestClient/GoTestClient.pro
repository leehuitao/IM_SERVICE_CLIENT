QT       += core gui network concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += /utf-8
QMAKE_LFLAGS += /utf-8

include($$PWD/utils/log/QsLog.pri)
include($$PWD/utils/voice/Voice.pri)
include($$PWD/utils/video/Video.pri)
include($$PWD/utils/Utils.pri)
include($$PWD/Network/Network.pri)


CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += HAVE_CONFIG_H
DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += WIN32_LEAN_AND_MEAN
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    File/fileitem.cpp \
    File/filewidget.cpp \
    Gui/ChatUi/emotion_widget.cpp \
    Gui/ChatUi/historical_user_list.cpp \
    Gui/ChatUi/invite_widget.cpp \
    Gui/ChatUi/message_box_item.cpp \
    Gui/ChatUi/message_interface.cpp \
    Gui/ChatUi/message_item.cpp \
    Gui/ChatUi/msg_listwidget_item.cpp \
    Gui/ChatUi/msg_widget_item.cpp \
    Gui/ChatUi/screenwidget.cpp \
    Gui/ChatUi/send_text_edit.cpp \
    Gui/ChatUi/video_widget.cpp \
    Gui/ChatUi/voice_widget.cpp \
    Gui/GroupUi/create_group_dialog.cpp \
    Gui/GroupUi/group_main_widget.cpp \
    Gui/HeadUi/head_dialog.cpp \
    Gui/HeadUi/head_widget.cpp \
    Gui/LoginUi/login_set_dialog.cpp \
    Gui/LoginUi/loginwidget.cpp \
    Gui/NewChatUi/chat_message.cpp \
    Gui/NewChatUi/new_chat_main.cpp \
    Gui/NewChatUi/new_chat_widget.cpp \
    Gui/OrgUi/org_main_widget.cpp \
    Gui/Setting/setting_dialog.cpp \
    Gui/system_set_dialog.cpp \
    Logic/app_cache.cpp \
    Logic/global_center.cpp \
    Logic/global_config.cpp \
    Logic/global_ui_center.cpp \
    main.cpp \
    mainwindow.cpp \
    sql/sqlite.cpp

HEADERS += \
    File/fileitem.h \
    File/filewidget.h \
    Gui/ChatUi/emotion_widget.h \
    Gui/ChatUi/historical_user_list.h \
    Gui/ChatUi/img_label.h \
    Gui/ChatUi/invite_widget.h \
    Gui/ChatUi/message_box_item.h \
    Gui/ChatUi/message_interface.h \
    Gui/ChatUi/message_item.h \
    Gui/ChatUi/msg_listwidget_item.h \
    Gui/ChatUi/msg_widget_item.h \
    Gui/ChatUi/screenwidget.h \
    Gui/ChatUi/send_text_edit.h \
    Gui/ChatUi/video_widget.h \
    Gui/ChatUi/voice_widget.h \
    Gui/GroupUi/create_group_dialog.h \
    Gui/GroupUi/group_main_widget.h \
    Gui/HeadUi/head_dialog.h \
    Gui/HeadUi/head_widget.h \
    Gui/LoginUi/login_set_dialog.h \
    Gui/LoginUi/loginwidget.h \
    Gui/NewChatUi/chat_message.h \
    Gui/NewChatUi/new_chat_main.h \
    Gui/NewChatUi/new_chat_widget.h \
    Gui/OrgUi/org_main_widget.h \
    Gui/Setting/setting_dialog.h \
    Gui/system_set_dialog.h \
    Logic/app_cache.h \
    Logic/global_center.h \
    Logic/global_config.h \
    Logic/global_define.h \
    Logic/global_ui_center.h \
    mainwindow.h \
    sql/db_define.h \
    sql/sqlite.h

FORMS += \
    File/fileitem.ui \
    File/filewidget.ui \
    Gui/ChatUi/emotion_widget.ui \
    Gui/ChatUi/invite_widget.ui \
    Gui/ChatUi/message_box_item.ui \
    Gui/ChatUi/message_interface.ui \
    Gui/ChatUi/msg_widget_item.ui \
    Gui/ChatUi/video_widget.ui \
    Gui/ChatUi/voice_widget.ui \
    Gui/GroupUi/create_group_dialog.ui \
    Gui/GroupUi/group_main_widget.ui \
    Gui/HeadUi/head_dialog.ui \
    Gui/HeadUi/head_widget.ui \
    Gui/LoginUi/login_set_dialog.ui \
    Gui/LoginUi/loginwidget.ui \
    Gui/NewChatUi/new_chat_main.ui \
    Gui/OrgUi/org_main_widget.ui \
    Gui/Setting/setting_dialog.ui \
    Gui/system_set_dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

