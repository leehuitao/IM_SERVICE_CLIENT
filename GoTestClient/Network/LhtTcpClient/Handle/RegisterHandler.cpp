#include "RegisterHandler.h"
#include <iostream>
#include <functional>
#include <shared_mutex>
RegisterHandler* RegisterHandler::instance_ = nullptr;
bool RegisterHandler::InitHandler(unsigned short cmd, RequestHandler handler)
{
    return MsgHandlerRepository<RequestHandler>::instance().registerHandler(cmd, handler);
}


