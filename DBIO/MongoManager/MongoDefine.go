package MongoManager

import (
	"context"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"strconv"
	"testserver/LogService"
	"testserver/settings"
	"time"
)

var mgoCli *mongo.Client
var databaseName string
var collectionMsg string
var collectionGroupMsg string

func InitEngine(config *settings.MongoConfig) {
	var err error
	databaseName = config.DatabaseName
	collectionMsg = config.CollectionMsg
	collectionGroupMsg = config.CollectionGroupMsg
	clientOptions := options.Client().ApplyURI("mongodb://" + config.Host + ":" + strconv.Itoa(config.Port)).
		SetConnectTimeout(1 * time.Second).
		SetServerSelectionTimeout(1 * time.Second). //设置超时时间
		SetMaxPoolSize(100)                         //设置最大连接池大小
	// 连接到MongoDB
	mgoCli, err = mongo.Connect(context.TODO(), clientOptions)
	if err != nil {
		LogService.Logger.Error(err.Error())
		return
	}
	// 检查连接
	err = mgoCli.Ping(context.TODO(), nil)
	if err != nil {
		LogService.Logger.Error(err.Error())
		return
	}
	LogService.Logger.Info("MongoDB 启动自动清除协程")
	go ScheduleFunction(AutoDelete)
	LogService.Logger.Info("MongoDB 连接成功")
}
func GetMgoCli() *mongo.Client {
	if mgoCli == nil {
		InitEngine(settings.Conf.MongoConfig)
	}
	return mgoCli
}

// ScheduleFunction 每天凌晨执行清除任务
func ScheduleFunction(fn func()) {
	for {
		// 获取当前时间
		now := time.Now()
		// 计算距离下一个凌晨12点的时间间隔
		nextMidnight := time.Date(now.Year(), now.Month(), now.Day()+1, 0, 0, 0, 0, now.Location())
		timeUntilMidnight := nextMidnight.Sub(now)

		// 等待到达凌晨12点
		time.Sleep(timeUntilMidnight)

		// 执行函数
		fn()
	}
}
