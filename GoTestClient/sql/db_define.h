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
#define InsertMsg "insert into HistoryMsg (MsgId,SendUserId,SendUserName,DstUserId,DstUserName,SendTime,MsgType,Msg,MsgStatus) "\
    "values ('%1',%2,'%3',%4,'%5','%6',%7,'%8',%9)"

#define UpdateMsgStatusQuery "UPDATE HistoryMsg SET MsgStatus = %2 WHERE MsgId = '%1'"

#define UpdateMsgType "SELECT * FROM HistoryMsg WHERE DstUserId == %1 and SendUserId == %2 and MsgStatus != 2;"

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


typedef QList<HistoryMsgStruct> HistoryMsgList;

#endif // DB_DEFINE_H
