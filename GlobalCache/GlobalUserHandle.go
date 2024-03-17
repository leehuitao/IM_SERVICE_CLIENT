// Package GlobalCache 全局人员管理方法
package GlobalCache

import (
	"encoding/json"
	"errors"
	"fmt"
	"testserver/DBIO/MysqlManager"
	"testserver/LogService"
	"testserver/PackManager"
)

var GlobalUsersInfoCache UsersInfoCache

func InitUserInfoCache() {
	GlobalUsersInfoCache.users = make(map[int]PackManager.GlobalUserInfo)
	GlobalUsersInfoCache.usersByName = make(map[string]PackManager.GlobalUserInfo)
	err := MysqlManager.SelectUsers(GlobalUsersInfoCache.users)
	if err != nil {
		return
	}
	for _, v := range GlobalUsersInfoCache.users {
		GlobalUsersInfoCache.usersByName[v.LoginName] = v
	}
	GlobalUsersInfoCache.usersNum = len(GlobalUsersInfoCache.users)

}
func (cache *UsersInfoCache) UpdateUserInfo(userId int) {
	userInfo := PackManager.GlobalUserInfo{}
	MysqlManager.GetUserInfo(userId, &userInfo)
	GlobalUsersInfoCache.users[userId] = userInfo
	GlobalUsersInfoCache.usersByName[userInfo.LoginName] = userInfo
}
func (cache *UsersInfoCache) GetInfoByName(name string) (error, PackManager.GlobalUserInfo) {
	if _, ok := GlobalUsersInfoCache.usersByName[name]; ok {
		// 键存在于 map 中
		return nil, GlobalUsersInfoCache.usersByName[name]
	} else {
		LogService.Logger.Error("不存在当前用户")
		return errors.New("不存在当前用户"), PackManager.GlobalUserInfo{}
	}
}
func (cache *UsersInfoCache) GetInfoById(id int) (error, PackManager.GlobalUserInfo) {
	if _, ok := GlobalUsersInfoCache.users[id]; ok {
		// 键存在于 map 中
		return nil, GlobalUsersInfoCache.users[id]
	} else {
		LogService.Logger.Error("不存在当前用户")
		return errors.New("不存在当前用户"), PackManager.GlobalUserInfo{}
	}
}
func (cache *UsersInfoCache) AddUser(user PackManager.GlobalUserInfo) {

	insertQuery := fmt.Sprintf("INSERT INTO user_table (UserId, UserName, LoginName, Password, "+
		"LastLogin, DeptId, PermissionLevel, CreateTime, CreateId) "+
		"VALUES "+
		"(%d, '%s', '%s', '%s', '%s', %d, %d, '%s', %d);",
		user.UserId, user.UserName, user.LoginName, user.Password, user.LastLogin,
		user.DeptId, user.PermissionLevel, user.CreateTime, user.CreateId)
	GlobalUsersInfoCache.users[user.CreateId] = user

	GlobalUsersInfoCache.usersByName[user.UserName] = user
	success := MysqlManager.QuerySql(insertQuery)
	if !success {

	}
	GlobalUsersInfoCache.usersNum = len(GlobalUsersInfoCache.users)
}

func (cache *UsersInfoCache) UpdateUser(user PackManager.GlobalUserInfo) {
	// 在这里更新用户信息到 cache.users
	updateQuery := fmt.Sprintf("UPDATE user_table SET "+
		"UserName = '%s', LoginName = '%s', Password = '%s',"+
		" LastLogin = '%s', DeptId = %d, PermissionLevel = %d, "+
		"CreateTime = '%s', CreateId = %d WHERE UserId = %d;",
		user.UserName, user.LoginName, user.Password, user.LastLogin, user.DeptId,
		user.PermissionLevel, user.CreateTime, user.CreateId, user.UserId)
	GlobalUsersInfoCache.users[user.UserId] = user
	GlobalUsersInfoCache.usersByName[user.UserName] = user
	success := MysqlManager.QuerySql(updateQuery)
	if !success {

	}
	GlobalUsersInfoCache.usersNum = len(GlobalUsersInfoCache.users)

}

func (cache *UsersInfoCache) DelUser(userId int) {
	// 在这里删除用户信息从 cache.users
	deleteQuery := fmt.Sprintf("DELETE FROM user_table WHERE UserId = %d;", userId)
	delete(GlobalUsersInfoCache.usersByName, GlobalUsersInfoCache.users[userId].UserName)
	delete(GlobalUsersInfoCache.users, userId)

	success := MysqlManager.QuerySql(deleteQuery)
	if !success {

	}
	GlobalUsersInfoCache.usersNum = len(GlobalUsersInfoCache.users)
}

func (cache *UsersInfoCache) GetAllUserJson() []byte {
	// 在这里获取所有用户的 JSON 数据
	jsonData := make(map[int]interface{})
	for k, v := range GlobalUsersInfoCache.users {
		b, _ := json.Marshal(v)
		jsonData[k] = b
	}
	marshal, err := json.Marshal(jsonData)
	if err != nil {
		return nil
	}
	return marshal
}

func (cache *UsersInfoCache) GetUserUsernameByLoginName(LoginName string) string {
	// 在这里根据 userId 获取用户密码

	return GlobalUsersInfoCache.usersByName[LoginName].UserName

}
func (cache *UsersInfoCache) GetUserPwdById(userId int) string {
	// 在这里根据 userId 获取用户密码
	if _, ok := GlobalUsersInfoCache.users[userId]; ok {
		// 键存在于 map 中
		return GlobalUsersInfoCache.users[userId].Password
	} else {
		LogService.Logger.Error("不存在当前用户")
		return ""
	}
}
func (cache *UsersInfoCache) GetUserPwdByName(userName string) (error, string) {
	// 在这里根据 userName 获取用户密码
	if _, ok := GlobalUsersInfoCache.usersByName[userName]; ok {
		// 键存在于 map 中
		return nil, GlobalUsersInfoCache.usersByName[userName].Password
	} else {
		return errors.New("不存在当前用户"), ""
	}
}

func (cache *UsersInfoCache) GetUserMacById(userId int) string {
	// 在这里根据 userId 获取用户 Mac 地址
	if _, ok := GlobalUsersInfoCache.users[userId]; ok {
		// 键存在于 map 中
		return ""
	} else {
		LogService.Logger.Error("不存在当前用户")
		return ""
	}
}

func (cache *UsersInfoCache) GetUserNameById(userId int) string {
	// 在这里根据 userId 获取用户中文名
	if _, ok := GlobalUsersInfoCache.users[userId]; ok {
		// 键存在于 map 中
		return GlobalUsersInfoCache.users[userId].LoginName
	} else {
		LogService.Logger.Error("不存在当前用户")
		return ""
	}
}

func (cache *UsersInfoCache) UpdateUserLoginTime(userId int, currentTime string) {
	// 在这里更新用户登录时间
	if _, ok := GlobalUsersInfoCache.users[userId]; ok {
		// 键存在于 map 中
		temp := GlobalUsersInfoCache.users[userId]
		temp.LastLogin = currentTime
		GlobalUsersInfoCache.users[userId] = temp
		updateQuery := fmt.Sprintf("UPDATE LastLogin = '%s',WHERE UserId = %d;", currentTime, userId)
		success := MysqlManager.QuerySql(updateQuery)
		if !success {

		}
	} else {
		LogService.Logger.Error("不存在当前用户")
	}
}
