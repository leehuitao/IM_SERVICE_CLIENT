package RedisManager

import (
	"strconv"
	"testserver/LogService"
)

const UserStatusKey = "UserStatusKey"

// ClearUserStatusKey 清空原来的缓存
func ClearUserStatusKey() {
	// 使用DEL命令删除指定的Redis键
	err := RedisClient.Del(UserStatusKey).Err()
	if err != nil {
		LogService.Logger.Error(err.Error())
	}

}

func AddOnlineUser(userid int) {
	err := RedisClient.HSet(UserStatusKey, strconv.Itoa(userid), 1).Err()
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
}

func UpdateUserStatus(userid, status int) {
	err := RedisClient.HSet(UserStatusKey, strconv.Itoa(userid), status).Err()
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
}

func DelOnlineUser(userid int) {
	err := RedisClient.HDel(UserStatusKey, strconv.Itoa(userid)).Err()
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
}

func GetUserStatus(userid int) int {
	val, err := RedisClient.HGet(UserStatusKey, strconv.Itoa(userid)).Result()
	if err != nil {
		LogService.Logger.Error(err.Error())
		return 0
	} else {
		status, _ := strconv.Atoi(val)
		return status
	}
}

func GetOnlineUsers() map[int]int {
	var ret map[int]int
	ret = make(map[int]int)
	// 获取哈希表中的所有字段（子键）
	fields, err := RedisClient.HKeys(UserStatusKey).Result()
	if err != nil {
		LogService.Logger.Error(err.Error())
		return nil
	}
	for _, userId := range fields {
		userIdInt, _ := strconv.Atoi(userId)
		ret[userIdInt] = GetUserStatus(userIdInt)
	}
	return ret
}
