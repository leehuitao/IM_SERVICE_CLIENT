package LogService

import (
	"github.com/natefinch/lumberjack"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"os"
	"testserver/settings"
)

var Logger *zap.Logger
var Level int

func InitLogger(config *settings.ZapLogConfig) {
	var coreArr []zapcore.Core

	//获取编码器
	encoderConfig := zap.NewProductionEncoderConfig()            //NewJSONEncoder()输出json格式，NewConsoleEncoder()输出普通文本格式
	encoderConfig.EncodeTime = zapcore.ISO8601TimeEncoder        //指定时间格式
	encoderConfig.EncodeLevel = zapcore.CapitalColorLevelEncoder //按级别显示不同颜色，不需要的话取值zapcore.CapitalLevelEncoder就可以了
	//encoderConfig.EncodeCaller = zapcore.FullCallerEncoder      	//显示完整文件路径
	encoder := zapcore.NewConsoleEncoder(encoderConfig)

	//info文件writeSyncer
	infoFileWriteSyncer := zapcore.AddSync(&lumberjack.Logger{
		Filename:   config.Filename,   //日志文件存放目录，如果文件夹不存在会自动创建
		MaxSize:    config.MaxSize,    //文件大小限制,单位MB
		MaxBackups: config.MaxBackups, //最大保留日志文件数量
		MaxAge:     config.MaxAge,     //日志文件保留天数
		Compress:   false,             //是否压缩处理
	})
	infoFileCore := zapcore.NewCore(encoder, zapcore.NewMultiWriteSyncer(infoFileWriteSyncer, zapcore.AddSync(os.Stdout)), zapcore.InfoLevel) //第三个及之后的参数为写入文件的日志级别,ErrorLevel模式只记录error级别的日志

	coreArr = append(coreArr, infoFileCore)
	Logger = zap.New(zapcore.NewTee(coreArr...), zap.AddCaller()) //zap.AddCaller()为显示文件名和行号，可省略

	//Logger.Info("hello info")
	//Logger.Debug("hello debug")
	//Logger.Error("hello error")
}
