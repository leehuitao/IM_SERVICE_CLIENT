#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

#include <QObject>

//连接属性
struct ConnectConfig {
    QString     ip;
    int         port;
};

//用户属性
struct UserConfig{
    //头像路径
    QString avatarPath;
    QString userLoginName;
    QString userPwd;
    //是否记住密码
    bool    rememberPwd;
    //登录后状态
    int     post_loginStatus = 1;
};

#endif // GLOBAL_DEFINE_H


