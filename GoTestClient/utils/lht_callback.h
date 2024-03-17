#ifndef LHT_CALLBACK_H
#define LHT_CALLBACK_H
#include <QByteArray>
class LhtCallback {
public:
    virtual bool lhtReadyRead(const QByteArray & arr) = 0;
};

#endif // LHT_CALLBACK_H
