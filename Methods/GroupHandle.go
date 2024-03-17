package Methods

import (
	"encoding/json"
	"testserver/GlobalCache"
	"testserver/PackManager"
)

func CreateGroup(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
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
