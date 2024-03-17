package main

import (
	"fmt"
	"strconv"
	"testserver/FileServer/FileMethods"
	"testserver/FileServer/FileNetwork"
	"testserver/LogService"
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
	settings.Conf.ZapLogConfig.Filename = "./log/FileServer.log"
	LogService.InitLogger(settings.Conf.ZapLogConfig)
	LogService.Logger.Info("日志服务启动")
	//方法注册
	FileMethods.Init()
	//网络模块启动
	var server FileNetwork.TcpServer
	fileListen := settings.Conf.FileServerConfig.Host + ":" + strconv.Itoa(settings.Conf.FileServerConfig.Port)
	LogService.Logger.Info("fileserver listen :" + fileListen)
	server.StartListen(fileListen)

	defer LogService.Logger.Sync()
}
