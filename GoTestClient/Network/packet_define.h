#ifndef PACKET_DEFINE_H
#define PACKET_DEFINE_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QObject>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include "sql/db_define.h"
#include "Network/LhtTcpClient/Handle/lht_file_manager.h"
#define APP_RUNNING_PATH QCoreApplication::applicationDirPath() + "/"
#define HEAD_IMAGE_PATH APP_RUNNING_PATH+"/headImage/"

#define     HeaderSize 12

#define     Login                       100
#define     Logout                      101
#define     SendMsg                     102
#define     GetMsgNotify                103
#define     GetMsg                      104
#define     UpdateMsgStatus             105
#define     MegHasBeenRead              106
#define 	GetOfflineMsg               107
#define     UpdateUserStatus            108
#define     GetUserHeadImage            109
#define     GetUserHeadImageData        110
#define     UploadUserHeadImage         111
#define     UploadUserHeadImageData     112
#define     UploadChatImage             113
#define     UploadChatImageData         114
#define     DownloadChatImage           115
#define     DownloadChatImageData       116

#define     GetMqttAddr                 120
#define     NotifyAudioStart            121
#define     NotifyAudioAccpt            122
#define     NotifyAudioStop             123

#define     GetVideoMqttAddr            130
#define     NotifyVideoStart            131
#define     NotifyVideoAccpt            132
#define     NotifyVideoStop             133

#define     OnlineUserList              200
#define     UpdateOnlineUser            201

#define     GetOrg                      300
#define     GetOnlineUser               301
#define     GetUserOrg                  302
#define     GetFileServerAddr           400
#define     UpdateConnectFileServerState    401
#define     ConnectFileServerSuccess    402
#define     FileFirstPack               501
#define     FileDataPack                502
#define     FileDataResPack             503
#define     FileCancelPack              504
#define     FilePausePack               505
#define     UserLogoffStatus    0
#define     UserLoginStatus     1
#define     UserLeaveStatus     2

#define ImageHeader "%$imgstart"
#define ImageEnd "%$imgend"
#define EmjHeader "%$emjstart"
#define EmjEnd "%$emjend"
#define ChatImagePath "./ChatImage/"
#define ShakeRecv "对方发送了一个抖动"
#define ShakeSend "您发送了一个抖动"

enum EmojiName{
    emoji1,
    emoji2,
    emoji3,
    emoji4,
    emoji5,
    emoji6,
    emoji7,
};

const QStringList EmojiPath{
    ":/resource/emoji/1.gif",
    ":/resource/emoji/2.gif",
    ":/resource/emoji/3.gif",
    ":/resource/emoji/4.gif",
    ":/resource/emoji/5.gif",
    ":/resource/emoji/6.gif",
    ":/resource/emoji/7.gif",
    ":/resource/emoji/8.gif",
    ":/resource/emoji/9.gif",
    ":/resource/emoji/10.gif",
    ":/resource/emoji/11.gif",
    ":/resource/emoji/12.gif",
    ":/resource/emoji/13.gif",
    ":/resource/emoji/14.gif",
    ":/resource/emoji/15.gif",
    ":/resource/emoji/16.gif",

};

static QString createFileMd5(QString filePath){
    QString sMd5;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray bArray = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
        sMd5 = QString(bArray.toHex()).toUpper();
    }
    file.close();
    return sMd5;
}

static bool copyFileToFolder(const QString &sourceFilePath, const QString &targetFolderPath) {
    QFileInfo srcFileInfo(sourceFilePath);
    if (!srcFileInfo.exists() || !srcFileInfo.isFile()) {
        qDebug() << "源文件不存在或不是一个文件。";
        return false;
    }

    QDir targetDir(targetFolderPath);
    // 检查目标文件夹是否存在，如果不存在则创建
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(".")) {
            qDebug() << "无法创建目标文件夹。";
            return false;
        }
    }
    auto md5 = createFileMd5(sourceFilePath);
    QString targetFilePath = targetFolderPath + QDir::separator() + md5 + "_" + srcFileInfo.fileName();
    QFile targetFile(targetFilePath);

    // 如果目标文件已存在，选择删除它或者返回失败
    if (targetFile.exists()) {
        qDebug() << "目标文件夹中已存在同名文件。";
        return false;
         //如果你想要覆盖它，取消下面代码的注释
         if (!targetFile.remove()) {
             qDebug() << "无法删除目标文件夹中的同名文件。";
             return false;
         }
    }

    // 进行复制操作
    if (!QFile::copy(sourceFilePath, targetFilePath)) {
        qDebug() << "文件复制失败。";
        return false;
    }

    return true;
}

static QString createUuid(){

}

struct MsgBody  {
    MsgBody(){}
    MsgBody(const HistoryMsgStruct & msg){
        this->MsgId             = msg.MsgId;
        this->Msg               = msg.Content;
        this->SendUserId        = msg.SendUserId;
        this->SendUserName      = msg.SendUserName;
        this->DstUserId         = msg.RecvUserId;
        this->DstUserName       = msg.RecvUserName;
        this->MsgType           = msg.MsgType;
        this->SendTime          = msg.SendTime;
        this->MsgStatus         = msg.MsgStatus;
    }
    QString     MsgId;//唯一消息ID
    int         UserId;
    int         SendUserId;
    QString     SendUserName;
    int         DstUserId;
    QString     DstUserName;
    QString     SendTime;//发送时间
    int         MsgType;//1：接收文件    2：发送文件 3:语音  4:视频
    int         MsgStatus = 0;//0：发送中  1：未读   2：已读
    QString     Msg;//消息内容
};

struct FileBody  {
    int             UserId     = 0;
    QString         FileName      ;
    QString         FileMD5       ;
    int             TotalSize  = 0;
    int             CurrentSize= 0;
    int             FileSeek   = 0;
    int             SendUserId = 0;
    int             RecvUserId = 0;
    bool            FilePause  = 0;
    QByteArray      FileData      ;
};

// LoginBody
struct LoginBody  {
    int         UserId     ;
    QString     UserName   ;
    QString     UserLoginName;
    QString     PassWord   ;
    bool        Notice     ;
    QString     MacAddress ;
    QString     LoginTime  ;
    int         LoginStatus = 0;
    QString     PhoneNumber  ;
    QString     Email  ;
    QString     HeadImagePath  ;
};

struct OnlineListBody  {
    int         UserId;
    int         Status;
};

struct SystemBody  {
    int         UserId;
    QString     SystemCMD;
};

// Header 协议头
struct Header  {
    //包大小
    int PackSize ;
    //指令
    int Method ;
    //指令类别
    int MethodType;
};

// Pack 协议体
struct Pack  {
    Header Header;
    //数据体
    QByteArray  Body;

    Pack(MsgBody body, int method ,int methodType){
        QJsonObject json;//构建json对象json
        json.insert("MsgId",body.MsgId);
        json.insert("UserId",body.UserId);
        json.insert("SendUserId", body.SendUserId);
        json.insert("SendUserName", body.SendUserName);
        json.insert("DstUserId", body.DstUserId);
        json.insert("DstUserName", body.DstUserName);
        json.insert("SendTime", body.SendTime);
        json.insert("MsgType", body.MsgType);
        json.insert("Msg", body.Msg);
        json.insert("MsgStatus", body.MsgStatus);
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        Body = byte_array;
        Header.Method = method;
        Header.MethodType = methodType;
        Header.PackSize =HeaderSize +   Body.size();
    }
    Pack(FileBody body, int method ,int methodType,bool isData = 0){
        if( isData == 0){
            QJsonObject json;//构建json对象json
            json.insert("UserId",body.UserId);
            json.insert("FileName", body.FileName);
            json.insert("FileMd5", body.FileMD5);
            json.insert("FileSize", body.TotalSize);
            json.insert("FileCurrentSize", body.CurrentSize);
            json.insert("FileCurrentSeek", body.FileSeek);
            json.insert("SendUserId", body.SendUserId);
            json.insert("RecvUserId", body.RecvUserId);
            json.insert("FilePause", body.FilePause);
            //json.insert("FileData", body.FileData.data());
            QJsonDocument document;
            document.setObject(json);
            QByteArray byte_array = document.toJson(QJsonDocument::Compact);
            Body = byte_array;
            Body.append(body.FileData);
            Header.Method = method;
            Header.MethodType = byte_array.size();
            Header.PackSize =HeaderSize +   Body.size();
        }
        else{
            QDataStream packet(&Body,QIODevice::WriteOnly);
//            QByteArray name(body.UserLoginName.toUtf8().data());
            QByteArray md5(body.FileMD5.toUtf8().data());
            packet<<body.UserId;
            packet<<md5.size();
//            Body +=  name;
            Body += md5;
            Body += body.FileData;
            Header.Method = method;
            Header.MethodType = methodType;
            Header.PackSize =HeaderSize +   Body.size();
        }
    }
    Pack(LoginBody body, int method ,int methodType){

        QJsonObject json;//构建json对象json
        json.insert("UserId",body.UserId);
        json.insert("UserName", body.UserName);
        json.insert("UserLoginName", body.UserLoginName);
        json.insert("PassWord", body.PassWord);
        json.insert("Notice", body.Notice);
        json.insert("MacAddress", body.MacAddress);
        json.insert("LoginTime", body.LoginTime);
        json.insert("LoginStatus", body.LoginStatus);
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        Body = byte_array;
        Header.Method = method;
        Header.MethodType = methodType;
        Header.PackSize =HeaderSize +   Body.size();


    }
    Pack(SystemBody body, int method ,int methodType){

        QJsonObject json;//构建json对象json
        json.insert("UserId", body.UserId);
        json.insert("SystemCMD", body.SystemCMD);
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        Body = byte_array;
        Header.Method = method;
        Header.MethodType = methodType;
        Header.PackSize =HeaderSize +   Body.size();
    }


    QByteArray toByte(){
        QByteArray m_buffer;
        QDataStream packet(&m_buffer,QIODevice::WriteOnly);
        packet<<Header.PackSize<<Header.Method<<Header.MethodType;
        m_buffer += Body;
//        qDebug()<<"m_buffer size = "<<m_buffer.size()<<"Body size = "<<Body.size();
        return m_buffer;
    }
};

struct NewPacket {
    Header header;
    QByteArray data;
};

typedef std::shared_ptr<NewPacket>     NewTcpProtocolPtr;
typedef std::function<void(NewTcpProtocolPtr)> RequestHandler;

#endif // PACKET_DEFINE_H
