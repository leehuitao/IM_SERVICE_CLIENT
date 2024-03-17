#pragma once
#include <QObject>
#include <QMutex>
#include "msghandler_repository.h"

class RegisterHandler
{

public:
    static RegisterHandler* Instance() {
        static QMutex mutex;
        if (!instance_) {
            QMutexLocker locker(&mutex);
            if (!instance_) {
                instance_ = new RegisterHandler();
            }
        }
        return instance_;
    }
    bool InitHandler(unsigned short cmd, RequestHandler handler);
private:
    static RegisterHandler* instance_;
};
