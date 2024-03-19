#include "mainwindow.h"
#include "Gui/LoginUi/loginwidget.h"
#include <QApplication>
#include "mainwindow_new.h"
#include "Logic/global_config.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void registerStruct(){
    qRegisterMetaType<LoginBody>("LoginBody");
    qRegisterMetaType<FileBody>("FileBody");
    qRegisterMetaType<MsgBody>("MsgBody");
    qRegisterMetaType<OnlineListBody>("OnlineListBody");
    qRegisterMetaType<SystemBody>("SystemBody");
    qRegisterMetaType<GroupBody>("GroupBody");
    qRegisterMetaType<QList<GroupStruct>>("QList<GroupStruct>");
    qRegisterMetaType<GroupStruct>("GroupStruct");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    registerStruct();

    Utils::Instance()->init();

    GlobalConfig::getInstance()->init();

    LoginWidget w;

    w.show();

    return a.exec();
}
