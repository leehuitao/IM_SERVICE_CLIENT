#include "packet_process.h"
#include <QDebug>
PacketProcess::PacketProcess()
{

}

LoginBody PacketProcess::parseLoginPack(QByteArray arr)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return LoginBody();
    }
    auto values = jsonDoc.object();
    LoginBody loginBody;
    loginBody.UserId = values.value("UserId").toInt();
    loginBody.UserLoginName = values.value("UserLoginName").toString();
    loginBody.UserName = values.value("UserName").toString();
    loginBody.PassWord = values.value("PassWord").toString();
    loginBody.Notice = values.value("Notice").toBool();
    loginBody.MacAddress = values.value("MacAddress").toString();
    loginBody.LoginTime = values.value("LoginTime").toString();
    loginBody.LoginStatus = values.value("LoginStatus").toInt();
    loginBody.PhoneNumber = values.value("PhoneNumber").toString();
    loginBody.Email = values.value("Email").toString();
    loginBody.HeadImagePath = values.value("HeadImagePath").toString();
    return loginBody;
}

MsgBody PacketProcess::parseMsgPack(QByteArray arr)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return MsgBody();
    }
    auto values = jsonDoc.object();
    MsgBody  msgBody;
    msgBody.MsgId           = values.value("MsgId").toString();
    msgBody.UserId          = values.value("UserId").toInt();
    msgBody.SendUserId      = values.value("SendUserId").toInt();
    msgBody.SendUserName    = values.value("SendUserName").toString();
    msgBody.DstUserId       = values.value("DstUserId").toInt();
    msgBody.DstUserName     = values.value("DstUserName").toString();
    msgBody.SendTime        = values.value("SendTime").toString();
    msgBody.MsgType         = values.value("MsgType").toInt();
    msgBody.Msg             = values.value("Msg").toString();
    msgBody.MsgStatus       = values.value("MsgStatus").toInt();
    return msgBody;
}

OnlineListBody PacketProcess::parseOnlineListBodyPack(QByteArray arr)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return OnlineListBody();
    }
    auto values = jsonDoc.object();
    OnlineListBody  onlineListBody;
    onlineListBody.UserId       = values.value("UserId").toInt();
    onlineListBody.Status       = values.value("Status").toInt();
    return onlineListBody;
}

SystemBody PacketProcess::parseSystemPack(QByteArray arr,int jsonSize)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return SystemBody();
    }
    auto values = jsonDoc.object();
    SystemBody  systemBody;
    systemBody.UserId           = values.value("UserId").toInt();
    systemBody.SystemCMD        = arr.mid(sizeof (systemBody.UserId),jsonSize);
    return systemBody;
}

FileBody PacketProcess::parseFileDataPack(QByteArray arr,int jsonsize)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc;

    if(jsonsize != 0)
        jsonDoc = (QJsonDocument::fromJson(arr.mid(0,jsonsize), &jsonError));
    else
        jsonDoc = (QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return FileBody();
    }
    auto values = jsonDoc.object();
    FileBody  fileBody;
    fileBody.UserId         =   values.value("UserId").toInt();
    fileBody.FileName       =   values.value("FileName").toString();
    fileBody.FileMD5        =   values.value("FileMd5").toString();
    fileBody.TotalSize      =   values.value("FileSize").toInt();
    fileBody.CurrentSize    =   values.value("FileCurrentSize").toInt();
    fileBody.FileSeek       =   values.value("FileCurrentSeek").toInt();
    fileBody.SendUserId     =   values.value("SendUserId").toInt();
    fileBody.RecvUserId     =   values.value("RecvUserId").toInt();
    fileBody.FilePause      =   values.value("FilePause").toBool();
    if(jsonsize != 0)
        fileBody.FileData   =   arr.mid(jsonsize,fileBody.CurrentSize);
    return fileBody;
}

FileBody PacketProcess::parseHeadImagePack(QByteArray arr, int size)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc;

    if(size != 0)
        jsonDoc = (QJsonDocument::fromJson(arr.mid(0,size), &jsonError));
    else
        jsonDoc = (QJsonDocument::fromJson(arr, &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        return FileBody();
    }
    auto values = jsonDoc.object();
    FileBody  fileBody;
    fileBody.UserId         =   values.value("UserId").toInt();
    fileBody.FileName       =   values.value("FileName").toString();
    fileBody.FileMD5        =   values.value("FileMd5").toString();
    fileBody.TotalSize      =   values.value("FileSize").toInt();
    fileBody.CurrentSize    =   values.value("FileCurrentSize").toInt();
    fileBody.FileSeek       =   values.value("FileCurrentSeek").toInt();
    if(size != 0)
        fileBody.FileData   =   arr.mid(size,fileBody.CurrentSize);
    return fileBody;
}

