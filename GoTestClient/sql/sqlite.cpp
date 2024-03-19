#include "sqlite.h"
#include <QDebug>
#include <QSqlQuery>
#include "Network/packet_define.h"
Sqlite::Sqlite(QObject *parent) : QObject(parent)
{

}

void Sqlite::initDB()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    auto dbName = APP_RUNNING_PATH + "user.db";
    m_database.setDatabaseName(dbName);
    m_database.open();
    if(m_database.lastError().type() != QSqlError::NoError){
        qDebug()<<"QSqlError = "<<m_database.lastError();
    }
    createDafultTable();
}
//#define insertMsg "insert into HistoryMsg (MsgId,SendUserId,SendUserName,DstUserId,DstUserName,SendTime,MsgType,Msg,MsgStatus) "
void Sqlite::insertHistoryMsg(MsgBody body)
{
    QSqlQuery query;
    QString str = QString(InsertMsg).arg(body.MsgId).arg(body.SendUserId).arg(body.SendUserName)
            .arg(body.DstUserId).arg(body.DstUserName)
            .arg(body.SendTime).arg(body.MsgType).arg(body.Msg).arg(body.MsgStatus);
//    qDebug()<<"str = "<<str;
    query.exec(str);
//    qDebug()<<__FUNCTION__<<query.lastError();
}

void Sqlite::updateMsgStatus(QString msgId, int status)
{
    QSqlQuery query;
    QString str = QString(UpdateMsgStatusQuery).arg(msgId).arg(status);
//    qDebug()<<"str = "<<str;
    query.exec(str);
//    qDebug()<<__FUNCTION__<<query.lastError();
}

int Sqlite::getUnreadNumber(int dstUserID,int myid)
{
    QSqlQuery query;
    QString str = QString(UpdateMsgType).arg(myid).arg(dstUserID);
//    qDebug()<<"str = "<<str;
    query.exec(str);
//    qDebug()<<__FUNCTION__<<query.lastError();
    int number = 0;
    while(query.next()){
        number++;
    }
    return number;
}

HistoryMsgList Sqlite::selectHistoryMsg(int sendUserId,int recvUserId)
{
    QSqlQuery query;
    QString str = QString(SelectMsg).arg(sendUserId).arg(recvUserId);
    qDebug()<<str;
    query.exec(str);
    HistoryMsgList list;
    while(query.next()){
        HistoryMsgStruct msg;
        msg.id = query.value(0).toInt();
        msg.MsgId = query.value(1).toString();
        msg.SendUserId = query.value(2).toInt();
        msg.SendUserName = query.value(3).toString();
        msg.RecvUserId = query.value(4).toInt();
        msg.RecvUserName = query.value(5).toString();
        msg.SendTime = query.value(6).toString();
        msg.MsgType = query.value(7).toInt();
        msg.Content = query.value(8).toString();
        msg.MsgStatus = query.value(9).toInt();
        list.insert(0,msg);
        qDebug()<<msg.Content;
    }
    qDebug()<<query.lastError()<<"str = "<<str;
    return list;
}

HistoryMsgList Sqlite::selectHistoryMsg(QString groupId)
{
    QSqlQuery query;
    QString str = QString(SelectGroupMsg).arg(groupId);
    qDebug()<<str;
    query.exec(str);
    HistoryMsgList list;
    while(query.next()){
        HistoryMsgStruct msg;
        msg.id = query.value(0).toInt();
        msg.MsgId = query.value(1).toString();
        msg.SendUserId = query.value(2).toInt();
        msg.SendUserName = query.value(3).toString();
        msg.RecvUserId = query.value(4).toInt();
        msg.RecvUserName = query.value(5).toString();
        msg.SendTime = query.value(6).toString();
        msg.MsgType = query.value(7).toInt();
        msg.Content = query.value(8).toString();
        msg.MsgStatus = query.value(9).toInt();
        list.insert(0,msg);
        qDebug()<<msg.Content;
    }
    qDebug()<<query.lastError()<<"str = "<<str;
    return list;
}

HistoryMsgList Sqlite::selectHistoryLastMsg()
{
    QSqlQuery query;
    QString str = QString(SelectLastMsg);
    qDebug()<<str;
    query.exec(str);
    HistoryMsgList list;
    while(query.next()){
        HistoryMsgStruct msg;
        msg.id = query.value(0).toInt();
        msg.MsgId = query.value(1).toString();
        msg.SendUserId = query.value(2).toInt();
        msg.SendUserName = query.value(3).toString();
        msg.RecvUserId = query.value(4).toInt();
        msg.RecvUserName = query.value(5).toString();
        msg.SendTime = query.value(6).toString();
        msg.MsgType = query.value(7).toInt();
        msg.Content = query.value(8).toString();
        msg.MsgStatus = query.value(9).toInt();
        list.insert(0,msg);
        qDebug()<<msg.Content;
    }
//    qDebug()<<query.lastError()<<"str = "<<str;
    return list;

}

void Sqlite::createDafultTable()
{
    QSqlQuery query;
    query.exec(CreateHistoryTable);
    qDebug()<<CreateHistoryTable;
    qDebug()<<query.lastError();
}
