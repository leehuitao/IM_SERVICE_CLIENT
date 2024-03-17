package Methods

import (
	"encoding/json"
	"fmt"
	"runtime"
	"strconv"
	"strings"
	"testserver/DBIO/MongoManager"
	"testserver/DBIO/MysqlManager"
	"testserver/FileServer/FileMethods"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/settings"
	"time"
)

func getCurrentFunctionName() string {
	pc, _, _, _ := runtime.Caller(1)
	funcName := runtime.FuncForPC(pc).Name()
	return funcName
}

//消息流程
//1.发送者发送消息服务器发送消息通知到对方客户端
//2.接收端收到通知后拉取消息详细数据
//3.接收端收到消息后发送已读通知到发送端

func initMongoBody(msgBody *PackManager.MsgBody, mongoBody *PackManager.MongoMsg) {
	mongoBody.MsgId = msgBody.MsgId
	mongoBody.SendUserId = msgBody.SendUserId
	mongoBody.SendUserName = msgBody.SendUserName
	mongoBody.ReceiverUserId = msgBody.DstUserId
	mongoBody.ReceiverUserName = msgBody.DstUserName
	mongoBody.Status = msgBody.MsgStatus
	mongoBody.Msg = msgBody.Msg
	mongoBody.SendTime = msgBody.SendTime
}
func initMsgBody(msgBody *PackManager.MsgBody, mongoBody *PackManager.MongoMsg) {
	msgBody.SendUserId = mongoBody.SendUserId
	msgBody.SendUserName = mongoBody.SendUserName
	msgBody.DstUserId = mongoBody.ReceiverUserId
	msgBody.DstUserName = mongoBody.ReceiverUserName
	msgBody.Msg = mongoBody.Msg
	msgBody.MsgStatus = mongoBody.Status
	msgBody.MsgId = mongoBody.MsgId
	msgBody.MsgType = 0
	msgBody.SendTime = mongoBody.SendTime
}

// SendMsg 发送文字消息
func SendMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		msgBody   PackManager.MsgBody
		mongoBody PackManager.MongoMsg
		msgId     string
	)

	if err := json.Unmarshal(pack.Body, &msgBody); err != nil {
		LogService.Logger.Error(err.Error())
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(msgBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(msgBody.UserId))
		return nil
	}
	currentTime := time.Now()
	// 格式化为 "年-月-日" 格式
	formattedTime := currentTime.Format("2006-01-02 15:04:05")
	//插入消息

	initMongoBody(&msgBody, &mongoBody)
	//插入mongodb后默认生成一个消息ID
	msgId = MongoManager.Insert(&mongoBody)
	queryStr := fmt.Sprintf(MysqlManager.InsertMessageRecord, msgId, msgBody.SendUserId, msgBody.SendUserName, msgBody.DstUserId, msgBody.DstUserName, msgBody.Msg, formattedTime)
	MysqlManager.Insert(queryStr)
	msgBody.MsgStatus = 1
	msgBody.SendTime = formattedTime
	getConn := ClientManagerHandle.GetConn(msgBody.DstUserId)
	pack.Header.MethodType = 1
	pack.Body, _ = json.Marshal(msgBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	pack.Header.MethodType = 0
	data = createSendBuffer(*pack)
	if getConn.conn != nil {
		getConn.conn.Write(data)
	}
	return pack
}

// GetMsgNotify 用户登录后拉取用户离线消息
func GetMsgNotify(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		msgBody   PackManager.MsgBody
		mongoBody []PackManager.MongoMsg
		msgIdList []string
	)

	if err := json.Unmarshal(pack.Body, &msgBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(msgBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(msgBody.UserId))
		return nil
	}
	mongoBody = MongoManager.SelectHistory(msgBody.UserId)

	for i := 0; i < len(mongoBody); i++ {
		msgIdList = append(msgIdList, mongoBody[i].Id)
	}
	msgId := strings.Join(msgIdList, ",")
	msgBody.MsgId = msgId
	pack.Body, _ = json.Marshal(msgBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data)
	}
	return pack
}

// GetMsg 拉取消息
func GetMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		msgBody   PackManager.MsgBody
		mongoBody PackManager.MongoMsg
	)

	if err := json.Unmarshal(pack.Body, &msgBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(msgBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(msgBody.UserId))
		return nil
	}

	//插入mongodb后默认生成一个消息ID
	mongoBody = *MongoManager.Select(msgBody.MsgId)
	initMsgBody(&msgBody, &mongoBody)
	pack.Body, _ = json.Marshal(msgBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data)
	}
	return pack
}

// UpdateMsgStatus 读取消息后更新消息状态
func UpdateMsgStatus(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		msgBody PackManager.MsgBody
	)

	if err := json.Unmarshal(pack.Body, &msgBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(msgBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(msgBody.UserId))
		return nil
	}
	//修改消息为已读
	MongoManager.Update(msgBody.MsgId, msgBody.MsgStatus)

	getConn := ClientManagerHandle.GetConn(msgBody.SendUserId)
	data := createSendBuffer(*pack)
	//回复消息
	if client.conn != nil {
		client.conn.Write(data)
	}
	pack.Header.Method = PackManager.MegHasBeenRead
	data = createSendBuffer(*pack)
	//发送已读通知到对方
	if getConn.conn != nil {
		getConn.conn.Write(data)
	}
	return pack
}

func GetOfflineMsg(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		sysBody PackManager.SystemBody
	)

	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}
	//修改消息为已读
	list := MongoManager.SelectHistory(sysBody.UserId)
	var body PackManager.MsgBody
	for _, val := range list {
		body.MsgId += val.MsgId + "|"
	}
	pack.Body, _ = json.Marshal(body)
	data := createSendBuffer(*pack)
	//回复消息
	if client.conn != nil {
		client.conn.Write(data)
	}

	return pack
}
func UploadChatImage(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	GlobalCache.InitWriteChatImageStream(taskInfo.UserId, taskInfo.FileName, taskInfo.FileMd5)
	resPack := PackManager.Pack{}
	resPack.Header.Method = PackManager.UploadChatImage
	resPack.Header.MethodType = 0
	resPack.Body = pack.Body
	data := createSendBuffer(resPack)
	client.conn.Write(data)
	return pack
}
func UploadChatImageData(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	f := GlobalCache.GetWriteChatImageStream(taskInfo.UserId)
	taskInfo.FileData = pack.Body[pack.Header.MethodType:]
	if f != nil {
		f.Seek(int64(taskInfo.FileCurrentSeek), 0)
		nLen, err := f.Write(taskInfo.FileData)
		len1 := int64(nLen)
		if err != nil || len1 != taskInfo.FileCurrentSize {
			LogService.Logger.Error("chat image file Write error")
			return
		}
	}
	pack.Header.Method = PackManager.UploadChatImageData
	taskInfo.FileCurrentSeek = taskInfo.FileCurrentSeek + taskInfo.FileCurrentSize
	if taskInfo.FileCurrentSeek == taskInfo.FileSize {
		taskInfo.FilePause = true
		GlobalCache.ClearWriteChatImageStream(taskInfo.UserId)
	}
	pack.Body, _ = json.Marshal(taskInfo)
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}
func DownloadChatImage(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	GlobalCache.InitReadChatImageStream(taskInfo.UserId, taskInfo.FileName, taskInfo.FileMd5)
	f := GlobalCache.GetReadChatImageStream(taskInfo.UserId)
	taskInfo.FileData = make([]byte, 8192)
	// 创建一个长度为8192的缓冲区
	if f != nil {
		f.Seek(int64(taskInfo.FileCurrentSeek), 0)
		nLen, err := f.Read(taskInfo.FileData)
		len1 := int64(nLen)
		// 获取文件状态信息
		fileInfo, _ := f.Stat()
		taskInfo.FileSize = fileInfo.Size()
		taskInfo.FileCurrentSize = len1
		if err != nil {
			LogService.Logger.Error("chat image file read error")
			return
		}
	}
	pack.Header.Method = PackManager.DownloadChatImageData
	if taskInfo.FileCurrentSeek+taskInfo.FileCurrentSize == taskInfo.FileSize {
		taskInfo.FilePause = true
		GlobalCache.ClearReadChatImageStream(taskInfo.UserId)
	}
	pack.Body, _ = json.Marshal(taskInfo)
	pack.Header.MethodType = len(pack.Body)
	data := createFileSendBuffer(*pack, taskInfo.FileData)
	client.conn.Write(data)
	return pack
}
func DownloadChatImageData(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	f := GlobalCache.GetReadChatImageStream(taskInfo.UserId)
	taskInfo.FileData = make([]byte, 8192)
	if f != nil {
		f.Seek(taskInfo.FileCurrentSeek, 0)
		len, err := f.Read(taskInfo.FileData)

		taskInfo.FileCurrentSize = int64(len)
		if err != nil {
			LogService.Logger.Error(err.Error())
			return
		}
	}
	pack.Header.Method = PackManager.DownloadChatImageData
	if taskInfo.FileCurrentSeek+taskInfo.FileCurrentSize == taskInfo.FileSize {
		taskInfo.FilePause = true
		GlobalCache.ClearReadChatImageStream(taskInfo.UserId)
	}
	pack.Body, _ = json.Marshal(taskInfo)
	pack.Header.MethodType = len(pack.Body)
	data := createFileSendBuffer(*pack, taskInfo.FileData)
	client.conn.Write(data)
	return pack
}

func GetMqttAddr(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	sysBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}
	addr := settings.Conf.MqttConfig.Host + ":" + strconv.Itoa(settings.Conf.MqttConfig.Port)
	pack.Body = []byte(addr)
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}

func NotifyAudioVideoHandle(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	var (
		msgBody PackManager.MsgBody
	)

	if err := json.Unmarshal(pack.Body, &msgBody); err != nil {
		LogService.Logger.Error(err.Error())
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(msgBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(msgBody.UserId))
		return nil
	}
	currentTime := time.Now()
	// 格式化为 "年-月-日" 格式
	formattedTime := currentTime.Format("2006-01-02 15:04:05")
	msgBody.MsgStatus = 2
	msgBody.SendTime = formattedTime
	getConn := ClientManagerHandle.GetConn(msgBody.DstUserId)
	if getConn.conn == nil {
		msgBody.MsgStatus = -1
	}
	pack.Header.MethodType = 1
	pack.Body, _ = json.Marshal(msgBody)
	data := createSendBuffer(*pack)
	if client.conn != nil {
		client.conn.Write(data) //response
	}
	pack.Header.MethodType = 0
	data = createSendBuffer(*pack)
	if getConn.conn != nil {
		getConn.conn.Write(data)
	}
	return pack
}
