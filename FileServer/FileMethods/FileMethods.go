package FileMethods

import (
	"encoding/binary"
	"fmt"
	"net"
	"strconv"
	"strings"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/Utils"
	"time"
)

// TaskInfo 任务详细信息
type TaskInfo struct {
	UserId          int    `json:"UserId"`
	FileName        string `json:"FileName"`
	FileMd5         string `json:"FileMd5"`
	FileSize        int64  `json:"FileSize"`
	FileCurrentSize int64  `json:"FileCurrentSize"`
	FileCurrentSeek int64  `json:"FileCurrentSeek"`
	SendUserId      int    `json:"SendUserId"`
	RecvUserId      int    `json:"RecvUserId"`
	FilePause       bool   `json:"FilePause"`
	FileData        []byte `json:"-"`
}
type FileClient struct {
	//ip
	Ip string
	//端口
	Port int
	//连接时间
	LoginTime time.Time
	//客户端socket
	Conn net.Conn
	//userid
	Userid int
	//用户名
	UserName string
}

// FileClientMap 客户端列表  userid -> 客户端
var FileClientMap map[int]FileClient

// FileTaskMap 文件任务列表   文件md5->文件详细信息
var FileTaskMap map[string]TaskInfo

func AddClient(client FileClient) {
	FileClientMap[client.Userid] = client
}

func DelClient(userid int) {
	if _, ok := FileClientMap[userid]; ok {
		delete(FileClientMap, userid)
	}
}

func AddTask(info TaskInfo) {
	FileTaskMap[info.FileMd5] = info
}
func DelTask(md5 string) {
	delete(FileTaskMap, md5)
}
func (client *FileClient) UpdateConn(conn net.Conn) {
	addr := conn.RemoteAddr().String()
	split := strings.Split(addr, ":")
	client.Ip = split[0]
	port, err := strconv.Atoi(split[1])
	if err != nil {
		fmt.Println("strconv.Atoi err", err)
		return
	}
	client.Port = port
	client.LoginTime = Utils.GetCurrentTime()
	client.Conn = conn
}
func parseHeader(header *PackManager.Header, sourceData []byte) {
	size := sourceData[:4]
	sizeInt := binary.BigEndian.Uint32(size)
	header.PackSize = int(sizeInt)
	cmd := sourceData[4:8]
	cmdInt := binary.BigEndian.Uint32(cmd)
	cmdLog := "rec cmd = " + strconv.Itoa(int(cmdInt))
	//LogService.LogDebug(cmdLog)
	LogService.Logger.Debug(cmdLog)
	header.Method = int(cmdInt)
	cmdType := sourceData[8:12]
	cmdTypeInt := binary.BigEndian.Uint32(cmdType)
	header.MethodType = int(cmdTypeInt)
}
func (client *FileClient) StartRead(conn net.Conn) {
	const bufSize = 40960 // 4096*10
	buffer := make([]byte, bufSize)
	var accumulatedSize int

	for {
		n, err := conn.Read(buffer[accumulatedSize:])
		if n == 0 {
			fmt.Println("客户端已关闭，断开连接")
			DelClient(client.Userid) // 删除tcp链接
			return
		}
		if err != nil {
			fmt.Println("conn Read err", err)
			return
		}
		accumulatedSize += n

		for {
			if accumulatedSize < PackManager.HeaderSize {
				break
			}

			var pack *PackManager.Pack
			pack = &PackManager.Pack{}
			parseHeader(&pack.Header, buffer)
			//如果数据长度小于包大小
			if accumulatedSize < pack.Header.PackSize {
				break
			}

			pack.Body = buffer[12:pack.Header.PackSize]
			go MethodPerform(pack, client)

			copy(buffer, buffer[pack.Header.PackSize:accumulatedSize])
			accumulatedSize -= pack.Header.PackSize
		}
	}
}

var FileMethodMap map[int]func(pack *PackManager.Pack, client *FileClient)
var FileNewChannelCache *FileChannelCache

// Register 注册方法
func Register() {
	FileMethodMap[PackManager.FileFirstPack] = FileFirstPack
	FileMethodMap[PackManager.FileDataPack] = FileDataPack
	FileMethodMap[PackManager.FileCancelPack] = FileCancelPack
	FileMethodMap[PackManager.FilePausePack] = FilePausePack
}

func MethodPerform(pack *PackManager.Pack, client *FileClient) {
	FileMethodMap[pack.Header.Method](pack, client)
}
