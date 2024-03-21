package Methods

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"net"
	"strconv"
	"strings"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/Utils"
	"time"
)

type TcpClient struct {
	//ip
	ip string
	//端口
	port int
	//连接时间
	loginTime time.Time
	//客户端socket
	conn net.Conn
	//userid
	Userid int
	//用户名
	UserName string
}

type ClientManager struct {
	//当前登录的个数
	currentClientNumber int
	//历史登录个数
	historyClientNumber int
	//userid -> client
	clientMap map[int]TcpClient
}

var ClientManagerHandle ClientManager
var newChannelCache *Utils.ChannelCache //= Utils.NewChannelCache()
//var userCache *UserCache.UserCache      //= UserCache.NewUserCache()

func ClientInit() {
	ClientManagerHandle.clientMap = make(map[int]TcpClient)

}
func (clientManager *ClientManager) AddConn(userid int, tcpClient TcpClient) {
	ClientManagerHandle.clientMap[userid] = tcpClient
}

func (clientManager *ClientManager) DelConn(userid int) {

	if client, ok := ClientManagerHandle.clientMap[userid]; ok {
		client.conn.Close()
	}
	delete(ClientManagerHandle.clientMap, userid)
}

func (clientManager *ClientManager) GetConn(userid int) TcpClient {
	client := clientManager.clientMap[userid]
	return client
}

func (clientManager *ClientManager) SendToConn(userid int, pack []byte) {
	client, ok := ClientManagerHandle.clientMap[userid]
	if ok {
		_, err := client.conn.Write(pack)
		if err != nil {
			LogService.Logger.Error("send to " + client.UserName + " client error")
			return
		}
	}
}

// UpdateConn 设置客户端连接以及其他信息
func (client *TcpClient) UpdateConn(conn net.Conn) {

	addr := conn.RemoteAddr().String()
	split := strings.Split(addr, ":")
	client.ip = split[0]
	port, err := strconv.Atoi(split[1])
	if err != nil {
		fmt.Println("strconv.Atoi err", err)
		return
	}
	client.port = port
	client.loginTime = Utils.GetCurrentTime()
	client.conn = conn
}

var number int

func (client *TcpClient) SetUserid(userid int) {
	client.Userid = userid
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

func (client *TcpClient) StartRead(conn net.Conn) {
	// 定义一个较大的缓冲区来存储累积的数据
	const bufSize = 40960 // 4096*10
	buffer := make([]byte, bufSize)
	var accumulatedSize int // 已累积数据的大小

	for {
		// 读取数据
		n, err := conn.Read(buffer[accumulatedSize:])
		if n == 0 {
			ClientManagerHandle.DelConn(client.Userid)                               //删除tcp链接
			GlobalCache.GlobalUserLoginStatus.DelOnlineUser(client.Userid)           //在线人员列表删除
			NoticeAllOnlineUserChangeStatus(client.Userid, PackManager.LogoffStatus) //通知所有人退出
			fmt.Println("客户端已关闭，断开连接")
			// 这里处理客户端关闭连接的逻辑
			return
		}
		if err != nil {
			fmt.Println("conn Read err", err)
			return
		}
		accumulatedSize += n

		// 循环处理累积的数据
		for {
			// 检查是否有足够的数据读取包头
			if accumulatedSize < PackManager.HeaderSize {
				break
			}

			// 解析包头
			var pack *PackManager.Pack
			pack = &PackManager.Pack{}
			parseHeader(&pack.Header, buffer)

			// 检查是否有足够的数据读取整个包
			if accumulatedSize < pack.Header.PackSize {
				break
			}

			// 读取完整的数据包
			pack.Body = make([]byte, pack.Header.PackSize-12)
			copy(pack.Body, buffer[12:pack.Header.PackSize])
			//pack.Body = buffer[12:pack.Header.PackSize]
			go MethodPerform(pack, client)

			// 移除已处理的数据
			copy(buffer, buffer[pack.Header.PackSize:accumulatedSize])
			accumulatedSize -= pack.Header.PackSize
		}
	}
}

func (client *TcpClient) SendData(data []byte) {
	client.conn.Write(data)
}

var MethodMap map[int]func(pack *PackManager.Pack, client *TcpClient) (request *PackManager.Pack)

func Init() {
	MethodMap = make(map[int]func(pack *PackManager.Pack, client *TcpClient) (request *PackManager.Pack))
	newChannelCache = Utils.NewChannelCache()
	Register()
	//客户端结构体初始化
	ClientInit()
}

// Register 注册方法
func Register() {
	//登录
	MethodMap[PackManager.Login] = SendLogin
	MethodMap[PackManager.Logout] = SendLogout
	//消息
	MethodMap[PackManager.SendMsg] = SendMsg
	MethodMap[PackManager.GetMsgNotify] = GetMsgNotify
	MethodMap[PackManager.GetMsg] = GetMsg
	MethodMap[PackManager.UpdateMsgStatus] = UpdateMsgStatus
	MethodMap[PackManager.GetOfflineMsg] = GetOfflineMsg
	//聊天文件
	MethodMap[PackManager.UploadChatImage] = UploadChatImage
	MethodMap[PackManager.UploadChatImageData] = UploadChatImageData
	MethodMap[PackManager.DownloadChatImage] = DownloadChatImage
	MethodMap[PackManager.DownloadChatImageData] = DownloadChatImageData
	//组织架构
	MethodMap[PackManager.GetOrg] = GetDeptOrg
	MethodMap[PackManager.GetOnlineUser] = GetOnlineUsers
	MethodMap[PackManager.GetUserOrg] = GetUserDeptOrg
	MethodMap[PackManager.UpdateUserStatus] = UpdateStatus
	//头像
	MethodMap[PackManager.GetUserHeadImage] = GetUserHeadImage
	MethodMap[PackManager.UploadUserHeadImage] = UploadUserHeadImage
	MethodMap[PackManager.UploadUserHeadImageData] = UploadUserHeadImageData
	//语音
	MethodMap[PackManager.GetMqttAddr] = GetMqttAddr
	MethodMap[PackManager.NotifyAudioStart] = NotifyAudioVideoHandle
	MethodMap[PackManager.NotifyAudioAccept] = NotifyAudioVideoHandle
	MethodMap[PackManager.NotifyAudioStop] = NotifyAudioVideoHandle
	//视频
	MethodMap[PackManager.GetVideoMqttAddr] = GetMqttAddr
	MethodMap[PackManager.NotifyVideoStart] = NotifyAudioVideoHandle
	MethodMap[PackManager.NotifyVideoAccept] = NotifyAudioVideoHandle
	MethodMap[PackManager.NotifyVideoStop] = NotifyAudioVideoHandle
	//文件
	MethodMap[PackManager.GetFileServerAddr] = GetFileServerAddr
	MethodMap[PackManager.UpdateConnectFileServerState] = UpdateConnectFileServerState
	//群组功能
	MethodMap[PackManager.CreateGroup] = CreateGroup
	MethodMap[PackManager.DelGroup] = DelGroup
	MethodMap[PackManager.UpdateGroup] = UpdateGroup
	MethodMap[PackManager.GroupRemoveMember] = GroupRemoveMember
	MethodMap[PackManager.GroupInviteMember] = GroupInviteMember
	MethodMap[PackManager.GroupSetAdmin] = GroupSetAdmin
	MethodMap[PackManager.GetUserGroupList] = GetUserGroupList
	MethodMap[PackManager.GetGroupUserList] = GetGroupUserList
	MethodMap[PackManager.SendGroupMsg] = SendGroupMsg
	MethodMap[PackManager.GetGroupMsg] = GetGroupMsg
	MethodMap[PackManager.GetGroupOfflineNotify] = GetGroupOfflineNotify
	MethodMap[PackManager.UpdateGroupMsgState] = UpdateGroupMsgState

}

func MethodPerform(pack *PackManager.Pack, client *TcpClient) (request *PackManager.Pack) {
	return MethodMap[pack.Header.Method](pack, client)
}

func intToBytes(n int) []byte {
	x := int32(n)
	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}

func createSendBuffer(pack PackManager.Pack) []byte {
	var buffer bytes.Buffer //Buffer是一个实现了读写方法的可变大小的字节缓冲
	buffer.Write(intToBytes(PackManager.HeaderSize + len(pack.Body)))
	buffer.Write(intToBytes(pack.Header.Method))
	buffer.Write(intToBytes(pack.Header.MethodType))
	buffer.Write(pack.Body)
	return buffer.Bytes()
}
func createFileSendBuffer(pack PackManager.Pack, fileData []byte) []byte {
	var buffer bytes.Buffer //Buffer是一个实现了读写方法的可变大小的字节缓冲
	buffer.Write(intToBytes(PackManager.HeaderSize + len(pack.Body) + len(fileData)))
	buffer.Write(intToBytes(pack.Header.Method))
	buffer.Write(intToBytes(pack.Header.MethodType))
	buffer.Write(pack.Body)
	buffer.Write(fileData)
	return buffer.Bytes()
}
func NoticeAllOnlineUserChangeStatus(Userid int, status int) {
	if Userid == 0 {
		return
	}
	pack := PackManager.Pack{}
	onlineListBody := PackManager.OnlineListBody{}
	onlineListBody.UserId = Userid
	onlineListBody.Status = status
	b, _ := json.Marshal(onlineListBody)
	pack.Body = b
	pack.Header.Method = PackManager.UpdateOnlineUser
	pack.Header.MethodType = 0
	data := createSendBuffer(pack)
	onlineByte := GlobalCache.GlobalUserLoginStatus.GetOnlineUserList()
	// 创建一个空的 map
	//var onlineUserLIST map[int]int
	//
	//// 将 JSON 字符串解析为 map
	//err := json.Unmarshal(onlineByte, &onlineUserLIST)
	//if err != nil {
	//	fmt.Println("Error:", err)
	//	return
	//}
	for k, _ := range onlineByte {
		ClientManagerHandle.SendToConn(k, data)
	}

}

func SendOnlineUserList(Userid int, conn net.Conn) (requestPack *PackManager.Pack) {
	//status := userCache.GetUserLoginStatus(UserName)
	online := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(Userid)
	if !online {
		return nil
	}
	onlineList := GlobalCache.GlobalUserLoginStatus.GetOnlineUserList()

	resPack := PackManager.Pack{}
	resPack.Header.Method = PackManager.GetOnlineUser
	resPack.Header.MethodType = 0
	resPack.Body, _ = json.Marshal(onlineList)
	data := createSendBuffer(resPack)
	conn.Write(data)
	return nil
}
