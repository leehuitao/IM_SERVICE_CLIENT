package main

import (
	"fmt"
	"go.uber.org/zap"
	"strconv"
	"testserver/DBIO/MongoManager"
	"testserver/DBIO/MysqlManager"
	"testserver/DBIO/RedisManager"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/Methods"
	"testserver/Network"
	"testserver/WebApis"
	"testserver/logger"
	"testserver/settings"
)

func main() {

	// 初始化配置
	if err := settings.InitConfiguration(); err != nil {
		panic(err)
	}

	if err := logger.InitLogConfig(settings.Conf.LogConfig, settings.Conf.Mode); err != nil {
		fmt.Printf("init logConfig failed, err: %v\n", err)
		return
	}
	//日志系统初始化
	LogService.InitLogger(settings.Conf.ZapLogConfig)
	LogService.Logger.Info("日志服务启动")
	//Redis初始化
	RedisManager.InitRedis(settings.Conf.RedisConfig)
	//Mongo初始化
	MongoManager.InitEngine(settings.Conf.MongoConfig)
	//Mysql连接池启动
	MysqlManager.StartMysqlService(settings.Conf.MysqlConfig)
	//数据库数据初始化
	GlobalCache.InitGlobalCache()
	//方法以及结构体注册
	Methods.Init()
	//启动web服务器
	webListen := settings.Conf.AppConfig.ListenIp + ":" + strconv.Itoa(settings.Conf.AppConfig.WebPort)
	LogService.Logger.Info(webListen)
	WebApis.NewApp(settings.Conf.AppConfig.ListenIp, settings.Conf.AppConfig.WebPort)
	//网络模块启动
	var server Network.TcpServer
	tcpListen := settings.Conf.AppConfig.ListenIp + ":" + strconv.Itoa(settings.Conf.AppConfig.TcpPort)
	LogService.Logger.Info(tcpListen)
	server.StartListen(tcpListen)

	defer zap.L().Sync()

	defer LogService.Logger.Sync()
}
