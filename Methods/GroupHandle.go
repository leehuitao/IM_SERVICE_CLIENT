package Methods

import (
	"encoding/json"
	"fmt"
	"log"
	"strconv"
	"strings"
	"testserver/DBIO/MongoManager"
	"testserver/DBIO/MysqlManager"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/Utils"
	"time"
)

const (
	GroupNormal = iota
	GroupAdmin
	GroupCreate
)

func CreateGroup(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	// 格式化为 "年-月-日" 格式
	currentTime := time.Now()
	formattedTime := currentTime.Format("2006-01-02 15:04:05")
	//第一步生成一个groupId
	uuid := Utils.CreateUuid()
	//插入数据库
	//1.群组表
	queryStr := fmt.Sprintf(MysqlManager.CreateNewGroupQuery, uuid, groupBody.GroupName, groupBody.GroupInfo, formattedTime, groupBody.UserId)
	err := MysqlManager.InsertLht(queryStr)
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
	queryStr = fmt.Sprintf(MysqlManager.AddGroupMemberQuery, uuid, groupBody.UserId, GroupCreate)
	err = MysqlManager.InsertLht(queryStr)
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
	//2.人员表
	var intArray []int
	strArray := strings.Split(groupBody.SendUserName, ",")
	for _, s := range strArray {
		i, err := strconv.Atoi(s)
		if err != nil || i == groupBody.UserId {
			fmt.Println("Error converting to int:", err)
			continue
		}
		intArray = append(intArray, i)
	}
	groupBody.GroupId = uuid
	pack.Body, _ = json.Marshal(groupBody)
	pack.Header.Method = PackManager.GroupInviteMember
	data := createSendBuffer(*pack)
	for _, it := range intArray {
		if it >= 88880000 {
			queryStr = fmt.Sprintf(MysqlManager.AddGroupMemberQuery, uuid, it, GroupNormal)
			err = MysqlManager.InsertLht(queryStr)
			if err != nil {
				LogService.Logger.Error(err.Error())
				continue
			}
			//通知所有被邀请的在线成员 (离线成员在登录时会拉取自己所在的群)
			getConn := ClientManagerHandle.GetConn(it)
			if getConn.conn != nil {
				getConn.conn.Write(data)
			}
		}
	}
	//返回包
	pack.Header.Method = PackManager.CreateGroup
	data = createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	return pack
}

func DelGroup(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		return nil
	}

	return pack
}

// UpdateGroup 更新群组信息
func UpdateGroup(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		return nil
	}

	return pack
}

// GroupRemoveMember 踢出成员
func GroupRemoveMember(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		return nil
	}

	return pack
}

// GroupInviteMember 邀请成员
func GroupInviteMember(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	//2.人员表
	var intArray []int
	strArray := strings.Split(groupBody.SendUserName, ",")
	for _, s := range strArray {
		i, err := strconv.Atoi(s)
		if err != nil || i == groupBody.UserId {
			fmt.Println("Error converting to int:", err)
			continue
		}
		intArray = append(intArray, i)
	}
	data := createSendBuffer(*pack)
	for it := range intArray {
		if it >= 88880000 {
			queryStr := fmt.Sprintf(MysqlManager.AddGroupMemberQuery, groupBody.GroupId, it, GroupNormal)
			err := MysqlManager.InsertLht(queryStr)
			if err != nil {
				LogService.Logger.Error(err.Error())
				continue
			}
			//通知所有被邀请的在线成员 (离线成员在登录时会拉取自己所在的群)
			getConn := ClientManagerHandle.GetConn(it)
			if getConn.conn != nil {
				getConn.conn.Write(data)
			}
		}
	}
	//返回包
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	return pack
}

// GroupSetAdmin 设置群组管理员
func GroupSetAdmin(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		return nil
	}

	return pack
}

// GetUserGroupList 获取群组列表
func GetUserGroupList(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	queryStr := fmt.Sprintf(MysqlManager.GetGroupsQuery, groupBody.UserId)
	rows := MysqlManager.GetRows(queryStr)
	defer rows.Close()
	var groups []PackManager.Group
	for rows.Next() {
		var g PackManager.Group

		if err := rows.Scan(&g.GroupID, &g.GroupName, &g.Announcement, &g.CreatedAt, &g.CreatorID); err != nil {
			log.Fatal(err)
		}

		groups = append(groups, g)
	}

	if err := rows.Err(); err != nil {
		log.Fatal(err)
	}

	// 序列化groups切片为JSON
	jsonGroups, err := json.Marshal(groups)
	if err != nil {
		log.Fatal(err)
	}
	groupBody.Msg = string(jsonGroups)
	//返回包
	pack.Body, _ = json.Marshal(groupBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	return pack
}

// GetGroupUserList 获取群组中的用户列表
func GetGroupUserList(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	queryStr := fmt.Sprintf(MysqlManager.GetGroupUsersQuery, groupBody.GroupId)
	rows := MysqlManager.GetRows(queryStr)
	defer rows.Close()
	var groupUsers []PackManager.GroupUsers
	for rows.Next() {
		var g PackManager.GroupUsers

		if err := rows.Scan(&g.Id, &g.GroupID, &g.UserId, &g.JoinAt, &g.UserLevel); err != nil {
			log.Fatal(err)
		}

		groupUsers = append(groupUsers, g)
	}

	if err := rows.Err(); err != nil {
		log.Fatal(err)
	}

	// 序列化groups切片为JSON
	jsonGroups, err := json.Marshal(groupUsers)
	if err != nil {
		log.Fatal(err)
	}
	groupBody.Msg = string(jsonGroups)
	//返回包
	pack.Body, _ = json.Marshal(groupBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	return pack
}

// SendGroupMsg 发送用户通知
func SendGroupMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	//插入mongodb后默认生成一个消息ID
	queryStr := fmt.Sprintf(MysqlManager.InsertMessageGroupRecord, groupBody.MsgId, groupBody.GroupId, groupBody.GroupName, groupBody.UserId, groupBody.SendUserName, groupBody.Msg)
	MysqlManager.Insert(queryStr)

	pack.Header.MethodType = 1
	groupBody.MsgStatus = 1
	pack.Body, _ = json.Marshal(groupBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	if v, ok := GlobalCache.GlobalGroupCache.Groups[groupBody.GroupId]; ok {
		insertMongoBody := PackManager.MongoGroupMsg{}
		insertMongoBody.MsgId = groupBody.MsgId
		insertMongoBody.UserId = groupBody.UserId
		insertMongoBody.SendUserId = groupBody.SendUserId
		insertMongoBody.SendUserName = groupBody.SendUserName
		insertMongoBody.GroupId = groupBody.GroupId
		insertMongoBody.GroupName = groupBody.GroupName
		insertMongoBody.SendTime = groupBody.SendTime
		insertMongoBody.MsgType = groupBody.MsgType
		insertMongoBody.MsgStatus = groupBody.MsgStatus
		insertMongoBody.Msg = groupBody.Msg
		for _, item := range v {
			if item.UserId != groupBody.UserId {
				insertMongoBody.RecvId = item.UserId
				_ = MongoManager.GroupInsert(&insertMongoBody)
				getConn := ClientManagerHandle.GetConn(item.UserId)
				if getConn.conn != nil {
					getConn.conn.Write(data)
				}
			}

		}
	}
	return pack
}
func initGroupMsgBody(msgBody *PackManager.GroupBody, mongoBody *PackManager.MongoGroupMsg) {
	msgBody.MsgId = mongoBody.MsgId
	msgBody.SendUserId = mongoBody.SendUserId
	msgBody.SendUserName = mongoBody.SendUserName
	msgBody.GroupId = mongoBody.GroupId
	msgBody.GroupName = mongoBody.GroupName
	msgBody.SendTime = mongoBody.SendTime
	msgBody.MsgType = mongoBody.MsgType
	msgBody.MsgStatus = mongoBody.MsgStatus
	msgBody.Msg = mongoBody.Msg
}

// GetGroupMsg 获取消息
func GetGroupMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		groupBody PackManager.GroupBody
		mongoBody PackManager.MongoGroupMsg
	)
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}

	//插入mongodb后默认生成一个消息ID
	mongoBody = *MongoManager.GroupSelect(groupBody.MsgId, groupBody.UserId)
	initGroupMsgBody(&groupBody, &mongoBody)
	pack.Body, _ = json.Marshal(groupBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data)
	}
	return pack
}
func UpdateGroupMsgState(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}
	//修改消息为已读
	MongoManager.GroupUpdate(groupBody.MsgId, groupBody.UserId, groupBody.MsgStatus)

	data := createSendBuffer(*pack)
	//回复消息
	if client.conn != nil {
		client.conn.Write(data)
	}
	return pack
}

// GetGroupOfflineNotify 获取离线群组消息
func GetGroupOfflineNotify(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	groupBody := PackManager.GroupBody{}
	if err := json.Unmarshal(pack.Body, &groupBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(groupBody.UserId)
	if !status {
		return nil
	}

	//修改消息为已读
	list := MongoManager.GroupSelectHistory(groupBody.UserId)
	var body PackManager.GroupBody
	for _, val := range list {
		body.MsgId += val.MsgId + "|"
	}
	pack.Body, _ = json.Marshal(body)
	data := createSendBuffer(*pack)
	//回复消息
	if client.conn != nil {
		client.conn.Write(data)
	}

	return pack
}
