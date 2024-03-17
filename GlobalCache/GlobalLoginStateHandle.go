// Package GlobalCache 用户管理用户状态
package GlobalCache

import (
	"testserver/DBIO/RedisManager"
)

var GlobalUserLoginStatus UserLoginState

func (uls *UserLoginState) AddOnlineUser(userId int) {
	RedisManager.AddOnlineUser(userId)
	//uls.UserLoginStatus[userId] = 1
	//uls.UserNum++
}

func (uls *UserLoginState) UpdateOnlineUser(userId, userStatus int) {
	RedisManager.UpdateUserStatus(userId, userStatus)
	//if uls.CheckUserIsOnline(userId) {
	//	uls.UserLoginStatus[userId] = userStatus
	//}
}

func (uls *UserLoginState) DelOnlineUser(userId int) {
	RedisManager.DelOnlineUser(userId)
	//if uls.CheckUserIsOnline(userId) {
	//	delete(uls.UserLoginStatus, userId)
	//	uls.UserNum--
	//}
}

func (uls *UserLoginState) GetOnlineUserList() (userStatus map[int]int) {
	return RedisManager.GetOnlineUsers()
	//userStatus, err := json.Marshal(GlobalUserLoginStatus.UserLoginStatus)
	//if err != nil {
	//	LogService.Logger.Error("GetOnlineUserList 格式化失败:" + err.Error())
	//	return nil
	//}
	//return userStatus
}

func (uls *UserLoginState) CheckUserIsOnline(userId int) bool {
	return RedisManager.GetUserStatus(userId) > 0
	//_, exists := uls.UserLoginStatus[userId]
	//return exists
}
