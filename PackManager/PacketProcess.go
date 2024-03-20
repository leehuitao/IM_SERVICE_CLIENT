package PackManager

import (
	"bytes"
	"encoding/binary"
)

const (
	Login                   = 100
	Logout                  = 101
	SendMsg                 = 102
	GetMsgNotify            = 103
	GetMsg                  = 104
	UpdateMsgStatus         = 105
	MegHasBeenRead          = 106
	GetOfflineMsg           = 107
	UpdateUserStatus        = 108
	GetUserHeadImage        = 109
	GetUserHeadImageData    = 110
	UploadUserHeadImage     = 111
	UploadUserHeadImageData = 112
	UploadChatImage         = 113
	UploadChatImageData     = 114
	DownloadChatImage       = 115
	DownloadChatImageData   = 116
	GetMqttAddr             = 120
	NotifyAudioStart        = 121
	NotifyAudioAccept       = 122
	NotifyAudioStop         = 123
	GetVideoMqttAddr        = 130
	NotifyVideoStart        = 131
	NotifyVideoAccept       = 132
	NotifyVideoStop         = 133

	CreateGroup                  = 150
	DelGroup                     = 151
	UpdateGroup                  = 152
	GroupRemoveMember            = 153
	GroupInviteMember            = 154
	GroupSetAdmin                = 155
	GetUserGroupList             = 156
	GetGroupUserList             = 157
	SendGroupMsg                 = 158
	GetGroupMsg                  = 159
	GetGroupOfflineNotify        = 160
	UpdateGroupMsgState          = 161
	UpdateOnlineUser             = 201
	GetOrg                       = 300
	GetOnlineUser                = 301
	GetUserOrg                   = 302
	GetFileServerAddr            = 400
	UpdateConnectFileServerState = 401
	ConnectFileServerSuccess     = 402
	FileFirstPack                = 501
	FileDataPack                 = 502
	FileDataResPack              = 503
	FileCancelPack               = 504
	FilePausePack                = 505
)
const (
	LogoffStatus     = 0
	LoginStatus      = 1
	LoginBusyStatus  = 2
	LoginLeaveStatus = 3
)

// Header -----------------一级协议包--------------------
const HeaderSize = 12

// Header 协议头
type Header struct {
	//包大小
	PackSize int
	//指令
	Method int
	//指令类别   文件服务中用作获取json数据大小   消息中用来判断是不是返回包  1 标识返回包
	MethodType int
}

// Pack 协议体
type Pack struct {
	Header Header
	//数据体
	Body []byte
}

// MsgBody -----------------二级协议包--------------------
// MsgBody 普通消息的json类型
type MsgBody struct {
	MsgId        string `json:"MsgId"`
	UserId       int    `json:"UserId"`
	SendUserId   int    `json:"SendUserId"`
	SendUserName string `json:"SendUserName"`
	DstUserId    int    `json:"DstUserId"`
	DstUserName  string `json:"DstUserName"`
	SendTime     string `json:"SendTime"`
	MsgType      int    `json:"MsgType"`
	Msg          string `json:"Msg"`
	MsgStatus    int    `json:"MsgStatus"`
}
type FileBody struct {
	UserId          int    `json:"UserId"`
	FileName        string `json:"FileName"`
	FileMd5         string `json:"FileMd5"`
	FileSize        int64  `json:"FileSize"`
	FileCurrentSize int64  `json:"FileCurrentSize"`
	FileCurrentSeek int64  `json:"FileCurrentSeek"`
	SendUserId      int    `json:"SendUserId"`
	RecvUserId      int    `json:"RecvUserId"`
	FilePause       bool   `json:"FilePause"`
}

// LoginBody  登录包json
type LoginBody struct {
	UserId        int    `json:"UserId"`
	UserName      string `json:"UserName"`
	UserLoginName string `json:"UserLoginName"`
	PassWord      string `json:"PassWord"`
	Notice        bool   `json:"Notice"`
	MacAddress    string `json:"MacAddress"`
	LoginTime     string `json:"LoginTime"`
	LoginStatus   int    `json:"LoginStatus"`
	ParentDeptID  int    `json:"ParentDeptID"`
	PhoneNumber   string `json:"PhoneNumber"`
	Email         string `json:"Email"`
	HeadImagePath string `json:"HeadImagePath"`
}

// SystemBody  系统指令json
type SystemBody struct {
	UserId    int    `json:"UserId"`
	SystemCMD string `json:"SystemCMD"`
}
type Group struct {
	GroupID      string `json:"group_id"`
	GroupName    string `json:"group_name"`
	Announcement string `json:"announcement"`
	CreatedAt    string `json:"created_at"`
	CreatorID    string `json:"creator_id"`
}
type GroupUsers struct {
	Id        int    `json:"id"`
	GroupID   string `json:"group_id"`
	UserId    int    `json:"user_id"`
	JoinAt    string `json:"join_at"`
	UserLevel int    `json:"user_level"`
}

// GroupBody  群组指令json
type GroupBody struct {
	MsgId        string `json:"MsgId" bson:"MsgId"`
	UserId       int    `json:"UserId" bson:"UserId"`
	SendUserId   int    `json:"SendUserId" bson:"SendUserId"`
	SendUserName string `json:"SendUserName" bson:"SendUserName"`
	GroupId      string `json:"GroupId" bson:"GroupId"`
	GroupName    string `json:"GroupName" bson:"GroupName"`
	GroupInfo    string `json:"GroupInfo" bson:"GroupInfo"`
	SendTime     string `json:"SendTime" bson:"SendTime"`
	MsgType      int    `json:"MsgType" bson:"MsgType"`
	MsgStatus    int    `json:"MsgStatus" bson:"MsgStatus"`
	Msg          string `json:"Msg" bson:"Msg"`
}

// OnlineListBody  用户状态更新json
type OnlineListBody struct {
	UserId int `json:"UserId"`
	Status int `json:"Status"`
}

type GlobalUserInfo struct {
	UserId          int    `json:"UserId"`
	UserName        string `json:"UserName"`
	LoginName       string `json:"LoginName"`
	Password        string `json:"Password"`
	LastLogin       string `json:"LastLogin"`
	DeptId          int    `json:"DeptId"`
	PermissionLevel int    `json:"PermissionLevel"`
	CreateTime      string `json:"CreateTime"`
	CreateId        int    `json:"CreateId"`
	PhoneNumber     string `json:"PhoneNumber"`
	Email           string `json:"Email"`
	HeadImagePath   string `json:"HeadImagePath"`
}

type DeptItem struct {
	DeptName     string `json:"DeptName"`
	DeptID       int    `json:"DeptID"`
	ParentDeptID int    `json:"ParentDeptID"`
	Level        int    `json:"Level"`
}

type MongoMsg struct {
	Id               string `bson:"-"`
	MsgId            string `bson:"MsgId"`
	SendUserId       int    `bson:"SendUserId"`
	SendUserName     string `bson:"SendUserName"`
	ReceiverUserId   int    `bson:"ReceiverUserId"`
	ReceiverUserName string `bson:"ReceiverUserName"`
	Msg              string `bson:"Msg"`
	Status           int    `bson:"Status"` //消息状态   0未读   1已读
	SendTime         string `bson:"SendTime"`
}
type MongoGroupMsg struct {
	Id           string `bson:"-"`
	MsgId        string `bson:"MsgId"`
	UserId       int    `bson:"UserId"`
	SendUserId   int    `bson:"SendUserId"`
	SendUserName string `bson:"SendUserName"`
	GroupId      string `bson:"GroupId"`
	GroupName    string `bson:"GroupName"`
	SendTime     string `bson:"SendTime"`
	RecvId       int    `bson:"RecvId"`
	MsgType      int    `bson:"MsgType"`
	MsgStatus    int    `bson:"MsgStatus"`
	Msg          string `bson:"Msg"`
}
type HeadImage struct {
	UserId          int    `json:"UserId"`
	FileName        string `json:"FileName"`
	FileMd5         string `json:"FileMd5"`
	FileSize        int64  `json:"FileSize"`
	FileCurrentSize int64  `json:"FileCurrentSize"`
	FileCurrentSeek int64  `json:"FileCurrentSeek"`
	FileData        []byte `json:"-"`
}

// IntToBytes 整形转换成字节
func IntToBytes(n int) []byte {
	x := int32(n)
	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}

// BytesToInt 字节转换成整形

func BytesToInt(b []byte) int {
	bytesBuffer := bytes.NewBuffer(b)

	var x int32
	binary.Read(bytesBuffer, binary.BigEndian, &x)

	return int(x)
}

func (proto *Pack) CreateBuffer() (data []byte) {
	buf := new(bytes.Buffer)
	if err := binary.Write(buf, binary.LittleEndian, proto); err != nil {
		return nil
	}
	return buf.Bytes()
}

func Decode(start int, size int, b []byte, dstByte []byte) {
	dstByte = b[start+12 : start+int(size)]
}
