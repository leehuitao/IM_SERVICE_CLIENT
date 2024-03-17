#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QMutex>

class Utils : public QObject
{
    Q_OBJECT
public:
    static Utils* Instance(){
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new Utils;
            }
        }
        return _instance;
    }
    void init();


private:
    explicit Utils(QObject *parent = nullptr);
    static Utils * _instance;
};

#endif // UTILS_H
