#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>
#include <QSettings>
#include "global_define.h"
#include "utils/utils.h"
/**
 * @brief 全局配置文件
 */
class GlobalConfig : public QObject
{
    Q_OBJECT
public:
    static GlobalConfig * getInstance(){
        return _instance;
    }

    void init(QString path = "");

    void setKeyValue(QString group , QString key, QVariant value);

    ConnectConfig getConnectConfig(){
        initConnect();
        return m_connectConfig;
    }

    UserConfig getCurrentUserConfig(){
        return m_currentUser;
    }
signals:


private:

    static GlobalConfig* _instance;
    explicit GlobalConfig(QObject *parent = nullptr);

    void initConnect();

    void initLoginUsers();

    void initCurrentUser();
private:
    QSettings   *       m_settings = nullptr;
    QString             m_configPath;
    ConnectConfig       m_connectConfig;
    QList<UserConfig>   m_users;
    UserConfig          m_currentUser;
};

#endif // GLOBALCONFIG_H
