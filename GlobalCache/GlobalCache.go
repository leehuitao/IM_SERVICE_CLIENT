// Package GlobalCache 全局数据以及用户的缓存
package GlobalCache

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"testserver/PackManager"
)

type UserInfoHandle interface {
	// GetInfoByName 获取用户数据
	GetInfoByName(name string) (error, PackManager.GlobalUserInfo)
	// AddUser web添加用户接口
	AddUser(info PackManager.GlobalUserInfo)
	// UpdateUser web 更新用户接口
	UpdateUser(info PackManager.GlobalUserInfo)
	// UpdateUserInfo 更新单个用户缓存
	UpdateUserInfo(userId int)
	// DelUser web删除用户接口
	DelUser(userId int)
	// GetAllUserJson 获取所有用户的json
	GetAllUserJson() []byte
	// GetUserUsernameByLoginName 获取用户名来自登录名   用于用户登录
	GetUserUsernameByLoginName(LoginName string) string
	// GetUserPwdById 获取用户密码来自id   用于用户登录
	GetUserPwdById(userId int) string
	// GetUserPwdByName 获取用户密码来自登录名   用于用户登录
	GetUserPwdByName(userName string) (error, string)
	// GetUserMacById 获取用户mac地址来自id 后期可用于mac绑定登录
	GetUserMacById(userId int) string
	// GetUserNameById 获取用户中文名  来自id
	GetUserNameById(userId int) string
	// UpdateUserLoginTime 更新用户登录时间
	UpdateUserLoginTime(userId int, currentTime string)
}
type UsersInfoCache struct {
	users       map[int]PackManager.GlobalUserInfo
	usersByName map[string]PackManager.GlobalUserInfo
	usersNum    int
	//每个用户的json数据
	UserJson map[int]interface{}
}
type DeptHandle interface {
	// AddDept 新增部门
	AddDept(data PackManager.DeptItem)
	// RemoveDept 删除部门
	RemoveDept(deptId int)
	// GetJson 获取组织架构json数据
	GetJson() []byte
	// OrgInit 组织架构初始化
	OrgInit()
}
type DeptCache struct {
	//部门的数据数组
	dept []PackManager.DeptItem
	//部门数组的json数据
	deptJson map[int]interface{}
}
type GroupCache struct {
	//群组数据
	Groups map[string][]PackManager.GroupUsers
}

type GroupHandle interface {
	// InitGroup 初始化群组
	InitGroup()
	// UpdateGroup 更新群组
	UpdateGroup(groupId string)
}
type UserLoginStateHandle interface {
	// AddOnlineUser 添加在线人员
	AddOnlineUser(userId int)
	// UpdateOnlineUser 修改在线人员状态
	UpdateOnlineUser(userId int, userStatus int)
	// DelOnlineUser 删除在线人员
	DelOnlineUser(userId int)
	// GetOnlineUserList 获取所有在线人员ID
	GetOnlineUserList() (num int, userList []int)
	// CheckUserIsOnline 检查人员是否在线
	CheckUserIsOnline(userId int)
}

// UserLoginState  用户登录状态管理结构
type UserLoginState struct {
	UserLoginStatus map[int]int
	//当前用户登录个数
	UserNum int
}

var FileMd5ToFileName map[string]string
var userIdToFileStream map[int]*os.File
var userIdToChatImageStream map[int]*os.File
var userIdToReadChatImageStream map[int]*os.File

func InitGlobalCache() {
	//组织结构初始化
	OrgInit()
	//群组结构初始化
	NewGroupCache()
	//用户状态初始化   修改为redis存储用户状态
	//InitUserLoginState()
	//人员数据初始化
	InitUserInfoCache()

	FileMd5ToFileName = make(map[string]string)

	userIdToFileStream = make(map[int]*os.File)

	userIdToChatImageStream = make(map[int]*os.File)

	userIdToReadChatImageStream = make(map[int]*os.File)
}

func InitFileStream(userId int, fileName string) {
	//打开文件，新建文件
	files := strings.Split(fileName, ".")
	filePath := "./headImage/" + strconv.Itoa(userId) + "." + files[len(files)-1]
	os.Truncate(filePath, 0)
	f, err := os.OpenFile(filePath, os.O_RDWR|os.O_APPEND|os.O_CREATE, os.ModePerm)
	if err != nil {
		fmt.Println("err = ", err)
		return
	}
	userIdToFileStream[userId] = f
}

func GetUserFileStream(userId int) *os.File {
	if f, ok := userIdToFileStream[userId]; ok {
		return f
	}
	return nil
}
func ClearFileStream(userId int) {
	if _, ok := userIdToFileStream[userId]; ok {
		err := userIdToFileStream[userId].Close()
		if err != nil {
			return
		}
		delete(userIdToFileStream, userId)
	}
}
func InitWriteChatImageStream(userId int, fileName string, md5 string) {
	//打开文件，新建文件
	filePath := "./imageCache/" + fileName
	os.Truncate(filePath, 0)
	f, err := os.OpenFile(filePath, os.O_RDWR|os.O_APPEND|os.O_CREATE, os.ModePerm)
	if err != nil {
		fmt.Println("err = ", err)
		return
	}
	userIdToChatImageStream[userId] = f
}
func GetWriteChatImageStream(userId int) *os.File {
	if f, ok := userIdToChatImageStream[userId]; ok {
		return f
	}
	return nil
}
func ClearWriteChatImageStream(userId int) {
	if _, ok := userIdToChatImageStream[userId]; ok {
		err := userIdToChatImageStream[userId].Close()
		if err != nil {
			return
		}
		delete(userIdToChatImageStream, userId)
	}
}

func InitReadChatImageStream(userId int, fileName string, md5 string) {
	//打开文件，新建文件
	filePath := "./imageCache/" + fileName
	//os.Truncate(filePath, 0)
	f, err := os.Open(filePath)
	if err != nil {
		fmt.Println("err = ", err)
		return
	}
	userIdToReadChatImageStream[userId] = f
}
func GetReadChatImageStream(userId int) *os.File {
	if f, ok := userIdToReadChatImageStream[userId]; ok {
		return f
	}
	return nil
}
func ClearReadChatImageStream(userId int) {
	if _, ok := userIdToReadChatImageStream[userId]; ok {
		err := userIdToReadChatImageStream[userId].Close()
		if err != nil {
			return
		}
		delete(userIdToReadChatImageStream, userId)
	}
}
