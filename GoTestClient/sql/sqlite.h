#ifndef SQLITE_H
#define SQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "db_define.h"
#include "Network/packet_define.h"
class Sqlite : public QObject
{
    Q_OBJECT
public:
    explicit Sqlite(QObject *parent = nullptr);

    void initDB();

    void insertHistoryMsg(MsgBody body);

    void insertGroupHistoryMsg(GroupBody body);

    void updateMsgStatus(QString msgId, int status);

    void updateGroupMsgStatus(QString msgId, int status);

    int getUnreadNumber(int dstUserID,int myid);

    int getUnreadNumber(QString groupId,int myid);

    HistoryMsgList selectHistoryMsg(int sendUserId,int recvUserId);

    HistoryGroupMsgList selectHistoryMsg(QString groupId);

    HistoryMsgList selectHistoryLastMsg();

    HistoryGroupMsgList selectHistoryLastGroupMsg();
signals:

public slots:

private:
    QSqlDatabase m_database;

private:
    void createDafultTable();
};

#endif // SQLITE_H
