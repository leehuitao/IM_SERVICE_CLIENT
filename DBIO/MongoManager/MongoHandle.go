package MongoManager

import (
	"context"
	"fmt"
	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/bson/primitive"
	"go.mongodb.org/mongo-driver/mongo"
	"log"
	"strconv"
	"testserver/LogService"
	"testserver/PackManager"
)

func Insert(data *PackManager.MongoMsg) string {
	var (
		client     = GetMgoCli()
		err        error
		collection *mongo.Collection
		iResult    *mongo.InsertOneResult
		id         primitive.ObjectID
	)
	//2.选择数据库里的表
	collection = client.Database(databaseName).Collection(collectionName)

	//插入一条数据
	if iResult, err = collection.InsertOne(context.TODO(), data); err != nil {
		fmt.Print(err)
		return ""
	}
	//_id:默认生成一个全局唯一ID
	id = iResult.InsertedID.(primitive.ObjectID)
	//fmt.Println("自增ID", id.Hex())
	return id.Hex()
}

func Select(id string) *PackManager.MongoMsg {
	var (
		client     = GetMgoCli()
		err        error
		collection *mongo.Collection
		cursor     *mongo.Cursor
	)
	lr := PackManager.MongoMsg{}

	collection = client.Database(databaseName).Collection(collectionName)
	filter := bson.M{"MsgId": id}

	//按照jobName字段进行过滤jobName="job10",翻页参数0-2
	if cursor, err = collection.Find(context.TODO(), filter); err != nil {
		fmt.Println(err)
		return &lr
	}
	//延迟关闭游标
	defer func() {
		if err = cursor.Close(context.TODO()); err != nil {
			log.Fatal(err)
		}
	}()

	//游标获取结果数据
	//if cursor != nil {
	//	//反序列化Bson到对象
	//	if err = cursor.Decode(&lr); err != nil {
	//		fmt.Print(err.Error())
	//		return &lr
	//	}
	//	//打印结果数据
	//	fmt.Println(lr)
	//}
	for cursor.Next(context.TODO()) {
		if err = cursor.Decode(&lr); err != nil {
			if err == mongo.ErrNoDocuments {
				fmt.Println("No more documents")
				break // 已经到达游标的末尾
			} else {
				log.Fatal(err)
			}
		}

		// 处理文档 lr
		fmt.Println(lr)
	}
	return &lr
}

func SelectHistory(recvId int) []PackManager.MongoMsg {
	var (
		client     = GetMgoCli()
		err        error
		collection *mongo.Collection
		cursor     *mongo.Cursor
	)
	var lr []PackManager.MongoMsg
	collection = client.Database(databaseName).Collection(collectionName)
	filter := bson.M{"ReceiverUserId": recvId, "Status": 1}

	//按照jobName字段进行过滤jobName="job10",翻页参数0-2
	if cursor, err = collection.Find(context.TODO(), filter); err != nil {
		fmt.Println(err)
		return lr
	}
	//延迟关闭游标
	defer func() {
		if err = cursor.Close(context.TODO()); err != nil {
			log.Fatal(err)
		}
	}()

	//游标获取结果数据

	for cursor.Next(context.TODO()) {
		var result PackManager.MongoMsg // 替换为您期望的数据结构类型
		if cursor.Decode(&result) != nil {
			fmt.Println(err)
			continue
		}
		lr = append(lr, result)
	}
	return lr
}

func Update(id string, status int) {
	var (
		client     = GetMgoCli()
		err        error
		collection *mongo.Collection
		iResult    *mongo.UpdateResult
	)
	// 更新条件
	filter := bson.M{"MsgId": id}

	// 更新内容
	update := bson.M{"$set": bson.M{"Status": status}}

	//2.选择数据库里的表
	collection = client.Database(databaseName).Collection(collectionName)

	// 执行更新
	iResult, _ = collection.UpdateOne(context.TODO(), filter, update)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("MatchedCount %v document(s) and modified %v document(s)\n", iResult.MatchedCount, iResult.ModifiedCount)
}

func AutoDelete() {
	var (
		client     = GetMgoCli()
		collection *mongo.Collection
		err        error
		uResult    *mongo.DeleteResult
		//upsertedID model.LogRecord
	)
	//2.选择数据库 my_db里的某个表
	collection = client.Database(databaseName).Collection(collectionName)
	filter := bson.M{"Status": "2"}
	if uResult, err = collection.DeleteMany(context.TODO(), filter); err != nil {
		LogService.Logger.Error(err.Error())
	}
	LogService.Logger.Info("清除通知表行数：" + strconv.Itoa(int(uResult.DeletedCount)))
}
