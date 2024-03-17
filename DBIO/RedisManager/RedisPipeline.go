// Package RedisManager 这里是管道处理的测试
package RedisManager

import (
	"fmt"
	"github.com/go-redis/redis"
	"time"
)

func pipelineDemo(client *redis.Client) {
	// 创建pipeline
	pipe := client.Pipeline()

	// 添加多个命令到pipeline
	setCmd := pipe.Set("key1", "value1", 0)
	getCmd := pipe.Get("key1")
	incrCmd := pipe.Incr("counter")
	pipe.Expire("key1", time.Hour)

	// 执行pipeline
	_, err := pipe.Exec()
	if err != nil {
		panic(err)
	}

	fmt.Println("setCmd:", setCmd.Val())
	fmt.Println("getCmd:", getCmd.Val())
	fmt.Println("incrCmd:", incrCmd.Val())
}
