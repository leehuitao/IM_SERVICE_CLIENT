package Methods

import (
	"encoding/json"
	"strconv"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
)

// GetDeptOrg 发送组织架构
func GetDeptOrg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	sysBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}

	//pack.Body = UserCache.OrgCacheData.GetJson()
	pack.Body = GlobalCache.GlobalDeptCache.GetJson()
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}

// GetUserDeptOrg 发送组织架构
func GetUserDeptOrg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	sysBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}

	pack.Body = GlobalCache.GlobalUsersInfoCache.GetAllUserJson()
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}

// GetOnlineUsers 发送在线人员列表
func GetOnlineUsers(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	sysBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}
	SendOnlineUserList(sysBody.UserId, client.conn)

	return pack
}
