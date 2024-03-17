package settings

import (
	"fmt"
	"github.com/fsnotify/fsnotify"
	"github.com/spf13/viper"
)

var Conf = new(MultipleConfig)

type MultipleConfig struct {
	*AppConfig        `mapstructure:"app"`
	*LogConfig        `mapstructure:"log"`
	*ZapLogConfig     `mapstructure:"zapLog"`
	*MysqlConfig      `mapstructure:"mysql"`
	*RedisConfig      `mapstructure:"redis"`
	*FileServerConfig `mapstructure:"fileServer"`
	*MongoConfig      `mapstructure:"mongoConfig"`
	*MqttConfig       `mapstructure:"mqttConfig"`
}
type ZapLogConfig struct {
	Level      string `mapstructure:"level"`
	Filename   string `mapstructure:"filename"`
	MaxSize    int    `mapstructure:"max_size"`
	MaxAge     int    `mapstructure:"max_age"`
	MaxBackups int    `mapstructure:"max_backups"`
}
type FileServerConfig struct {
	Host   string `mapstructure:"host"`
	Port   int    `mapstructure:"port"`
	Weight int    `mapstructure:"weight"`
}

type AppConfig struct {
	Name     string `mapstructure:"name"`
	Mode     string `mapstructure:"mode"`
	Version  string `mapstructure:"version"`
	ListenIp string `mapstructure:"listenIp"`
	TcpPort  int    `mapstructure:"tcpPort"`
	WebPort  int    `mapstructure:"webPort"`
}

type LogConfig struct {
	Level      string `mapstructure:"level"`
	Filename   string `mapstructure:"filename"`
	MaxSize    int    `mapstructure:"max_size"`
	MaxAge     int    `mapstructure:"max_age"`
	MaxBackups int    `mapstructure:"max_backups"`
}

type MysqlConfig struct {
	Host         string `mapstructure:"host"`
	Port         int    `mapstructure:"port"`
	User         string `mapstructure:"user"`
	Password     string `mapstructure:"password"`
	DbName       string `mapstructure:"dbname"`
	MaxOpenConns int    `mapstructure:"max_open_conns"`
	MaxIdleConns int    `mapstructure:"max_Idle_conns"`
}

type RedisConfig struct {
	Host     string `mapstructure:"host"`
	Port     int    `mapstructure:"port"`
	Password string `mapstructure:"password"`
	Db       int    `mapstructure:"db"`
	PoolSize int    `mapstructure:"pool_size"`
}

type MongoConfig struct {
	Host           string `mapstructure:"host"`
	Port           int    `mapstructure:"port"`
	DatabaseName   string `mapstructure:"databaseName"`
	CollectionName string `mapstructure:"collectionName"`
}
type MqttConfig struct {
	Host string `mapstructure:"host"`
	Port int    `mapstructure:"port"`
}

func InitConfiguration() (err error) {
	viper.SetConfigFile("config/config.yaml")
	err = viper.ReadInConfig()
	if err != nil {
		panic(fmt.Errorf("Init config file failed, err: %v\n", err))
		return
	}

	//反序列化到结构体
	if err := viper.Unmarshal(Conf); err != nil {
		fmt.Printf("viper unmarshal failed, err: %v\n", err)
	}

	// 监控并重新读取配置文件
	viper.WatchConfig()
	viper.OnConfigChange(func(in fsnotify.Event) {
		fmt.Println("config file is changed...", in.Name)
		if err := viper.Unmarshal(Conf); err != nil {
			fmt.Printf("viper unmarshal failed, err: %v\n", err)
		}
	})
	return
}
