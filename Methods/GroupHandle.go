package Methods

import (
	"encoding/json"
	"fmt"
	"strconv"
	"strings"
	"testserver/DBIO/MysqlManager"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/Utils"
	"time"
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
	queryStr = fmt.Sprintf(MysqlManager.AddGroupMemberQuery, uuid, groupBody.UserId)
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
	for it := range intArray {
		if it >= 88880000 {
			queryStr = fmt.Sprintf(MysqlManager.AddGroupMemberQuery, uuid, it)
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
			queryStr := fmt.Sprintf(MysqlManager.AddGroupMemberQuery, groupBody.GroupId, it)
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
	pack.Body, _ = json.Marshal(groupBody)
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

// GetGroupUserList 获取群组中的用户列表
func GetGroupUserList(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
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

// SendGroupMsg 发送用户通知
func SendGroupMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
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

// GetGroupMsg 获取消息
func GetGroupMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
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

// GetGroupOfflineNotify 获取离线群组消息
func GetGroupOfflineNotify(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
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
