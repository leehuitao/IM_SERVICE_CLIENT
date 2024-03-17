package FileMethods

import (
	"encoding/json"
	"fmt"
	"net"
	"os"
	"sync"
	"testserver/LogService"
	"testserver/PackManager"
	"time"
)

type FileChannelCache struct {
	FileDataChanMap  map[string]chan TaskInfo
	FileStopChanMap  map[string]chan bool
	FilePauseChanMap map[string]chan bool
}

func InitFileNewChannelCache() (Cache *FileChannelCache) {
	Cache = &FileChannelCache{
		make(map[string]chan TaskInfo),
		make(map[string]chan bool),
		make(map[string]chan bool),
	}
	return Cache
}

var Mutex sync.Mutex

func (channelCache *FileChannelCache) AddNewChannelCache(fileMd5 string) {
	Mutex.Lock()
	defer Mutex.Unlock()
	channelCache.FileDataChanMap[fileMd5] = make(chan TaskInfo, 10000)
	channelCache.FileStopChanMap[fileMd5] = make(chan bool)
	channelCache.FilePauseChanMap[fileMd5] = make(chan bool)

}

func (channelCache *FileChannelCache) ClearChannelCache(fileMd5 string) {
	Mutex.Lock()
	defer Mutex.Unlock()
	delete(channelCache.FileDataChanMap, fileMd5)
	delete(channelCache.FileStopChanMap, fileMd5)
	delete(channelCache.FilePauseChanMap, fileMd5)
}

func createPack(data TaskInfo) []byte {
	pack := PackManager.Pack{}
	//设置一下包头
	pack.Header.Method = PackManager.FileDataPack
	marshal, _ := json.Marshal(data)
	pack.Header.MethodType = len(marshal)
	//合并两个切片设置到body
	pack.Body = marshal
	return createSendBuffer(pack, data.FileData)
}

// FileTransferHandle 实现简单的带缓存的文件传输
func FileTransferHandle(info TaskInfo, recvConn net.Conn) bool {
	//打开文件，新建文件
	os.Truncate("./"+info.FileMd5+"_"+info.FileName, 0)
	f, err := os.OpenFile("./"+info.FileMd5+"_"+info.FileName, os.O_RDWR|os.O_APPEND|os.O_CREATE, os.ModePerm)
	if err != nil {
		fmt.Println("err = ", err)
		return false
	}
	data := FileNewChannelCache.FileDataChanMap[info.FileMd5]
	defer close(data)
	defer f.Close()
	for {
		select {
		case filePack := <-data: //接受数据
			if &filePack != nil {
				f.Seek(int64(filePack.FileCurrentSeek), 0)
				nLen, err := f.Write(filePack.FileData)
				len1 := int64(nLen)
				if err != nil || len1 != filePack.FileCurrentSize {
					FileNewChannelCache.ClearChannelCache(info.FileMd5)
					LogService.Logger.Error("file Write error")
					return false
				} else {
					byteData := createPack(filePack)
					_, err := recvConn.Write(byteData)
					if err != nil {
						LogService.Logger.Error(err.Error())
						return false
					}
					if filePack.FileCurrentSeek+filePack.FileCurrentSize >= info.FileSize {
						f.Close()
						FileNewChannelCache.ClearChannelCache(info.FileMd5)
						LogService.Logger.Info("文件传输完成")
						return true
					}
				}
			} else {
				LogService.Logger.Error("file data error")
				FileNewChannelCache.ClearChannelCache(info.FileMd5)
				return true
			}
		case <-time.After(time.Minute * 3): //三分钟内没反应退出协程
			FileNewChannelCache.ClearChannelCache(info.FileMd5)
			LogService.Logger.Error(info.FileMd5 + "_" + info.FileName + " timeout ")
			return false
		case <-FileNewChannelCache.FilePauseChanMap[info.FileMd5]: //用于暂停协程
			<-FileNewChannelCache.FilePauseChanMap[info.FileMd5] // Waiting for the unpause signal
		case <-FileNewChannelCache.FileStopChanMap[info.FileMd5]: //退出协程
			FileNewChannelCache.ClearChannelCache(info.FileMd5)
			LogService.Logger.Info(info.FileMd5 + "_" + info.FileName + "received complete channel quit")
			pack := PackManager.Pack{}
			//设置一下包头
			pack.Header.Method = PackManager.FileCancelPack
			marshal, _ := json.Marshal(info)
			pack.Header.MethodType = len(marshal)
			//合并两个切片设置到body
			pack.Body = marshal
			f.Write(createSendBuffer(pack, nil))
			f.Close()
			return true
			//default:
			//	//fmt.Println("No data available. Waiting...")
			//	time.Sleep(1 * time.Second) // 在没有数据时进行延迟

		}
	}

}
