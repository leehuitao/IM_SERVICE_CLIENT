#include "global_config.h"
#include <QCoreApplication>
GlobalConfig* GlobalConfig::_instance = new  GlobalConfig;

void GlobalConfig::init(QString path)
{
    if(path.isEmpty()){
        m_configPath = QCoreApplication::applicationDirPath()+"/config/config.ini";
    }
    if(m_settings){
        delete m_settings;
    }
    m_settings = new QSettings(m_configPath,QSettings::IniFormat);

    initConnect();

    initCurrentUser();
}

void GlobalConfig::setKeyValue(QString group, QString key, QVariant value)
{
    m_settings->setValue(group+"/"+key,value);
}

GlobalConfig::GlobalConfig(QObject *parent) : QObject(parent)
{

}

void GlobalConfig::initConnect()
{
    m_settings->beginGroup("CONNECT");
    m_connectConfig.ip = m_settings->value("ip").toString();
    m_connectConfig.port = m_settings->value("port").toInt();
    m_settings->endGroup();
}

void GlobalConfig::initLoginUsers()
{
    m_settings->beginGroup("Users");
    m_connectConfig.ip = m_settings->value("ip").toString();
    m_connectConfig.port = m_settings->value("port").toInt();
    m_settings->endGroup();
}

void GlobalConfig::initCurrentUser()
{
    m_settings->beginGroup("CurrentUser");
    m_currentUser.avatarPath        = m_settings->value("avatarPath").toString();
    m_currentUser.userLoginName     = m_settings->value("userLoginName").toString();
    m_currentUser.userPwd           = m_settings->value("userPwd").toString();
    m_currentUser.rememberPwd       = m_settings->value("rememberPwd").toBool();
    m_currentUser.post_loginStatus  = m_settings->value("post_loginStatus").toInt();
    m_settings->endGroup();
}
