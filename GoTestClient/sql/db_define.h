#ifndef DB_DEFINE_H
#define DB_DEFINE_H
#include <QObject>
#define CreateHistoryTable "CREATE TABLE 'HistoryMsg' (" \
    "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,        "\
    "MsgId TEXT NOT NULL,                                  "\
    "SendUserId integer NOT NULL,                          "\
    "SendUserName TEXT NOT NULL,                           "\
    "DstUserId integer NOT NULL,                           "\
    "DstUserName TEXT NOT NULL,                            "\
    "SendTime TEXT NOT NULL,                               "\
    "MsgType integer NOT NULL,                             "\
    "Msg TEXT NOT NULL,                                    "\
    "MsgStatus integer NOT NULL                            "\
    ");"

#define CreateHistoryGroupMsgTable "CREATE TABLE HistoryGroupMsg (" \
"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,  " \
"GroupId text NOT NULL,                          " \
"SendUserId INTEGER NOT NULL,                    " \
"SendUserName TEXT NOT NULL,                     " \
"Content TEXT NOT NULL,                          " \
"MsgId text NOT NULL,                            " \
"GroupName TEXT NOT NULL,                        " \
"SendTime text NOT NULL,                         " \
"MsgType integer NOT NULL,                       " \
"MsgStatus integer NOT NULL                      " \
");"

#define InsertMsg "insert into HistoryMsg (MsgId,SendUserId,SendUserName,DstUserId,DstUserName,SendTime,MsgType,Msg,MsgStatus) "\
    "values ('%1',%2,'%3',%4,'%5','%6',%7,'%8',%9)"

#define InsertGroupMsg "INSERT INTO HistoryGroupMsg (GroupId, SendUserId, SendUserName, Content, MsgId, GroupName, SendTime,MsgType,MsgStatus) "\
    "VALUES ('%1', %2, '%3', '%4', '%5', '%6', '%7',%8,%9)"

#define UpdateMsgStatusQuery "UPDATE HistoryMsg SET MsgStatus = %2 WHERE MsgId = '%1'"

#define UpdateGroupMsgStatusQuery "UPDATE HistoryGroupMsg SET MsgStatus = %2 WHERE MsgId = '%1'"

#define UpdateMsgType "SELECT * FROM HistoryMsg WHERE DstUserId == %1 and SendUserId == %2 and MsgStatus != 2;"

#define SelectGroupMsgType "SELECT * FROM HistoryGroupMsg WHERE GroupId == '%1' and SendUserId != %2 and MsgStatus != 2;"

#define SelectMsg "select * from HistoryMsg where (DstUserId = %1 and SendUserId = %2) or (DstUserId = %2 and SendUserId = %1) order by id DESC limit 20;"

#define SelectGroupMsg "select * from HistoryGroupMsg where GroupId = '%1' order by id DESC limit 20;"


#define SelectLastMsg "SELECT A.* "\
    "FROM HistoryMsg AS A "\
    "INNER JOIN ( "\
    "    SELECT SenderUserId, ReceiverUserId, MAX(SendTime) AS LatestSendTime "\
    "    FROM ( "\
    "        SELECT SendUserId AS SenderUserId, DstUserId AS ReceiverUserId, SendTime "\
    "        FROM HistoryMsg "\
    "        UNION ALL "\
    "        SELECT DstUserId AS SenderUserId, SendUserId AS ReceiverUserId, SendTime "\
    "        FROM HistoryMsg "\
    "    ) AS CombinedData "\
    "    GROUP BY SenderUserId, ReceiverUserId "\
    ") AS B "\
    "ON (A.SendUserId = B.SenderUserId AND A.DstUserId = B.ReceiverUserId AND A.SendTime = B.LatestSendTime) ORDER BY SendTime DESC"


#define SelectLastGroupMsg "SELECT * FROM ( SELECT id,GroupId,SendUserId,SendUserName,Content,MsgId, GroupName,SendTime,"\
        "MsgType,MsgStatus,ROW_NUMBER( ) OVER ( PARTITION BY GroupId ORDER BY SendTime DESC ) AS rn"\
        " FROM HistoryGroupMsg) t WHERE t.rn = 1;"

struct HistoryMsgStruct{
    int             id;
    QString         MsgId;
    QString         SendUserName;
    int             SendUserId;
    QString         RecvUserName;
    int             RecvUserId;
    QString         Content;
    QString         SendTime;
    int             MsgStatus;
    int             MsgType;
};

struct HistoryGroupMsgStruct{
    int             id;
    QString         GroupId;
    int             SendUserId;
    QString         SendUserName;
    QString         Content;
    QString         MsgId;
    QString         GroupName;
    QString         SendTime;
    int             MsgType;
    int             MsgStatus;
};

typedef QList<HistoryMsgStruct> HistoryMsgList;

typedef QList<HistoryGroupMsgStruct> HistoryGroupMsgList;
#endif // DB_DEFINE_H
