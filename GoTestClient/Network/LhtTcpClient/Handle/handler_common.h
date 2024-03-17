#ifndef HANDLER_COMMON_H
#define HANDLER_COMMON_H
#include <iostream>
#include <QObject>
#include "../../packet_define.h"
class HandlerCommon: public std::enable_shared_from_this<HandlerCommon>
{
public:
    virtual void handleMessage(NewTcpProtocolPtr msg) = 0;

protected:

    virtual void OperratingException(const std::size_t exception_code) {}

    virtual void SendResponseMessage() {};

};
#endif // HANDLER_COMMON_H
