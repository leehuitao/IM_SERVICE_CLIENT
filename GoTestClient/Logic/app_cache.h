#ifndef APPCACHE_H
#define APPCACHE_H

#include <QMap>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>
#include <QVector>
#include "Network/packet_define.h"

static QString getCurrentTime(){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss-zzz");
    return current_date;
}

struct GlobalUserInfo
{
    int     UserId;
    QString UserName;
    int     DeptId;
};

struct UserStruct{
    QString  UserName       ;
    QString  UserLoginName  ;
    int      ParentDeptId   ;
};

struct DeptStruct{
    QString     DeptName     ;
    int         DeptId       ;
    int         ParentDeptId ;
    int         Level        ;
};

struct GroupInvitationStruct{
    QString     GroupName;
    QString     GroupInfo;
    QList<int>  users;
};

class AppCache: public QObject
{
public:

    Q_OBJECT
public:
    static AppCache* Instance(){
        static QMutex mutex;
        if (!instance_) {
            QMutexLocker locker(&mutex);
            if (!instance_) {
                instance_ = new AppCache;
            }
        }
        return instance_;
    }
    QString                     m_userName;
    QString                     m_userLoginName;
    QString                     m_passWord;
    QString                     m_currentFilePath;
    int                         m_userId = 0;
    int                         m_msgSize=0;
    QString                     m_userHeadImagePath;
    QMap<int,QString>           m_headImagePath;
    QMap<int,LoginBody>         m_userInfos;
    QMap<QString,GroupBody>     m_groupInfos;
    QMap<QString,QList<GroupUsersStruct>>    m_groupUsers;
    QString m_fileSavePath="D:/ceshi/";
private:
    AppCache();
private:
    static AppCache*        instance_;
};

#endif // APPCACHE_H
