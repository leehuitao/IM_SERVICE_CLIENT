#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include <QObject>
#include "RegisterHandler.h"
class HandlerManager : public QObject
{
    Q_OBJECT
public:
    static HandlerManager* Instance();
    //初始化
     void init();

    template<class Handler>
    bool InitHandler(const unsigned short cmd);
private:

    template<class Handler>
    void NotifyHandler(NewTcpProtocolPtr msg);


private:
    bool registered_;
    static  HandlerManager*g_instance ;
    explicit HandlerManager(QObject *parent = nullptr);


signals:


private:
    //初始化接收方法
    void initRecvMethod();
    //初始化返回包统一接口
    void initResponseMethod();
    //初始化音视频
    void initVideoMethod();
    //初始化消息接收
    void initMsgMethod();
    //初始化群组接收
    void initGroupMethod();
    //初始化文件发送
    void initSendFileMethod();
};

#endif // HANDLERMANAGER_H
