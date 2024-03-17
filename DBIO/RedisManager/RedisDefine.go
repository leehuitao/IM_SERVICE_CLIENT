package RedisManager

import (
	"github.com/go-redis/redis"
	"strconv"
	"testserver/LogService"
	"testserver/settings"
)

var (
	RedisClient *redis.Client
)

func InitRedis(config *settings.RedisConfig) {
	// 创建连接池
	RedisClient = redis.NewClient(&redis.Options{
		Addr:     config.Host + ":" + strconv.Itoa(config.Port), // Redis 服务器地址
		Password: config.Password,                               // 密码
		PoolSize: config.PoolSize,                               // 连接池中的最大连接数
		DB:       config.Db,
	})
	_, err := RedisClient.Ping().Result()
	if err != nil {
		LogService.Logger.Error(err.Error())
	}
	LogService.Logger.Info("Redis 连接成功")

	ClearUserStatusKey()
}
