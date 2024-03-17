package FileMethods

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"testserver/LogService"
	"testserver/PackManager"
)

func Init() {
	FileClientMap = make(map[int]FileClient)
	FileTaskMap = make(map[string]TaskInfo)
	FileMethodMap = make(map[int]func(pack *PackManager.Pack, client *FileClient))
	FileNewChannelCache = InitFileNewChannelCache()
	Register()
}
func intToBytes(n int) []byte {
	x := int32(n)
	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}
func createSendBuffer(pack PackManager.Pack, fileData []byte) []byte {
	var buffer bytes.Buffer //Buffer是一个实现了读写方法的可变大小的字节缓冲
	buffer.Write(intToBytes(PackManager.HeaderSize + len(pack.Body) + len(fileData)))
	buffer.Write(intToBytes(pack.Header.Method))
	buffer.Write(intToBytes(pack.Header.MethodType))
	buffer.Write(pack.Body)
	buffer.Write(fileData)
	return buffer.Bytes()
}

// FileFirstPack 客户端发送的第一个包
// 发送端包含接收或者发送的文件名/md5/用户id等
// 接收端只需要用户id 登录文件服务即可
func FileFirstPack(pack *PackManager.Pack, client *FileClient) {
	taskInfo := TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("FileFirstPack 解析json数据失败")
		return
	}
	client.Userid = taskInfo.UserId
	AddClient(*client)
	if taskInfo.UserId == taskInfo.SendUserId {
		if _, ok := FileNewChannelCache.FileDataChanMap[taskInfo.FileMd5]; ok {
			LogService.Logger.Error("任务已存在")
			return
		} else {
			//添加任务
			AddTask(taskInfo)
			//初始化channel通道
			FileNewChannelCache.AddNewChannelCache(taskInfo.FileMd5)
		}

	} else if taskInfo.UserId == taskInfo.RecvUserId {
		//开启文件传输协程
		LogService.Logger.Info("file go thread started")
		if _, ok := FileNewChannelCache.FileDataChanMap[taskInfo.FileMd5]; ok {
			go FileTransferHandle(taskInfo, client.Conn)
		} else {
			LogService.Logger.Error("任务不存在")
		}

	} else {
		LogService.Logger.Error("FileFirstPack UserId 错误")
		return
	}
	pack.Body, _ = json.Marshal(taskInfo)
	data := createSendBuffer(*pack, nil)
	_, err := client.Conn.Write(data)
	if err != nil {
		LogService.Logger.Error("FileDataPack 返回数据包错误")
		return
	}
}

func FileDataPack(pack *PackManager.Pack, client *FileClient) {
	taskInfo := TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("FileDataPack 解析json数据失败")
		return
	}
	//taskInfo.FileData = pack.Body[pack.Header.MethodType:]
	originalSlice := pack.Body[pack.Header.MethodType:]
	// 创建一个新的切片，长度和容量与需要复制的数据一致
	copiedSlice := make([]byte, len(originalSlice))
	// 使用 copy 函数复制数据
	copy(copiedSlice, originalSlice)
	// 将复制后的切片赋值给 taskInfo.FileData
	taskInfo.FileData = copiedSlice

	if taskInfo.UserId == taskInfo.SendUserId {
		//向channel中传入数据
		Mutex.Lock()
		defer Mutex.Unlock()
		if _, ok := FileNewChannelCache.FileDataChanMap[taskInfo.FileMd5]; ok {
			FileNewChannelCache.FileDataChanMap[taskInfo.FileMd5] <- taskInfo
		}

	}
	pack.Header.Method = PackManager.FileDataResPack
	taskInfo.FileCurrentSeek = taskInfo.FileCurrentSeek + taskInfo.FileCurrentSize
	pack.Body, _ = json.Marshal(taskInfo)
	data := createSendBuffer(*pack, nil)
	_, err := client.Conn.Write(data)
	if err != nil {
		LogService.Logger.Error("FileDataPack 返回数据包错误")
		return
	}
	// else if taskInfo.UserId == taskInfo.RecvUserId {
	//	if v, ok := FileClientMap[taskInfo.SendUserId]; ok {
	//		pack.Header.Method = PackManager.FileDataResPack
	//		taskInfo.FileCurrentSeek = taskInfo.FileCurrentSeek + taskInfo.FileCurrentSize
	//		pack.Body, _ = json.Marshal(taskInfo)
	//		data := createSendBuffer(*pack, nil)
	//		_, err := v.Conn.Write(data)
	//		if err != nil {
	//			LogService.Logger.Error("FileDataPack 返回数据包错误")
	//			return
	//		}
	//	}
	//
	//}

}
func FileCancelPack(pack *PackManager.Pack, client *FileClient) {
	taskInfo := TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("FileDataPack 解析json数据失败")
		return
	}

	//向channel中传入数据
	FileNewChannelCache.FileStopChanMap[taskInfo.FileMd5] <- true

}
func FilePausePack(pack *PackManager.Pack, client *FileClient) {
	taskInfo := TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("FileDataPack 解析json数据失败")
		return
	}
	//向channel中传入数据
	FileNewChannelCache.FilePauseChanMap[taskInfo.FileMd5] <- taskInfo.FilePause
}
