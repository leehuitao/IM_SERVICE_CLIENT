// Package RedisManager 这里是消息队列的测试
package RedisManager

import (
	"fmt"
	"github.com/go-redis/redis"
	"math/rand"
	"time"
)

var client *redis.Client

// 初始化连接
func initClient() {
	client = redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "",
		DB:       0,
	})
}

// 生产者 - 发布消息
func producer() {
	for {
		message := rand.Intn(1000)
		err := client.LPush("queue", message).Err()
		if err != nil {
			panic(err)
		}
		fmt.Println("pushed", message)
		time.Sleep(1 * time.Second)
	}
}

// 消费者 - 处理消息
func consumer(id int) {
	for {
		message, err := client.BRPop(0, "queue").Result()
		if err != nil {
			panic(err)
		}
		fmt.Printf("consumer%d popped %s \n", id, message[1])
		time.Sleep(500 * time.Millisecond)
	}
}

func testQueue() {
	// 初始化
	initClient()

	// 生产者goroutine
	go producer()

	// 3个消费者goroutine
	for i := 0; i < 3; i++ {
		go consumer(i)
	}

	// 阻塞主goroutine
	<-make(chan struct{})
}
