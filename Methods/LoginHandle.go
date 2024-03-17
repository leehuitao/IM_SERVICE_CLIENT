package Methods

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
	"testserver/DBIO/MysqlManager"
	"testserver/FileServer/FileMethods"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"time"
)

// SendLogin 登录
func SendLogin(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.LoginBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	err, password := GlobalCache.GlobalUsersInfoCache.GetUserPwdByName(loginBody.UserLoginName)

	resPack := PackManager.Pack{}
	resPack.Header.Method = pack.Header.Method
	resPack.Header.MethodType = 0

	if err != nil {
		client.conn.Close()
		loginBody.LoginStatus = 0
	}
	if loginBody.PassWord == password {
		loginBody.LoginStatus = 1

		//用户名获取 用户所有信息
		// 获取当前时间
		currentTime := time.Now()
		// 格式化为 "年-月-日" 格式
		formattedTime := currentTime.Format("2006-01-02 15:04:05")
		_, userInfo := GlobalCache.GlobalUsersInfoCache.GetInfoByName(loginBody.UserLoginName)
		//插入登录记录
		queryStr := fmt.Sprintf(MysqlManager.InsertLoginRecord, userInfo.UserId, userInfo.UserName, formattedTime, client.ip)
		MysqlManager.Insert(queryStr)
		NoticeAllOnlineUserChangeStatus(userInfo.UserId, PackManager.LoginStatus)
		GlobalCache.GlobalUserLoginStatus.AddOnlineUser(userInfo.UserId)
		loginBody.UserId = userInfo.UserId
		loginBody.UserName = userInfo.UserName
		loginBody.PhoneNumber = userInfo.PhoneNumber
		loginBody.Email = userInfo.Email
		loginBody.HeadImagePath = userInfo.HeadImagePath
		loginBody.LoginTime = formattedTime
		//在线客户端Map增加当前客户端
		client.Userid = userInfo.UserId
		client.UserName = userInfo.UserName
		ClientManagerHandle.AddConn(userInfo.UserId, *client)
		//userName, err := userCache.GetUserName(loginBody.UserLoginName)
		if err != nil {
			return nil
		}
		//loginBody.UserName = userInfo.UserName

	} else {
		loginBody.LoginStatus = 0
	}

	b, _ := json.Marshal(loginBody)
	resPack.Body = b
	data := createSendBuffer(resPack)
	client.conn.Write(data)
	return pack
}
func GetUserHeadImage(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	//status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	//if !status {
	//	return nil
	//}
	go sendImage2User(loginBody.UserId, loginBody.SystemCMD, client)
	return pack
}

// SendLogout 登出
func SendLogout(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.LoginBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		return nil
	}
	resPack := PackManager.Pack{}
	resPack.Header.Method = pack.Header.Method
	resPack.Header.MethodType = 0
	resPack.Body = []byte{}
	data := createSendBuffer(resPack)
	client.conn.Write(data)
	ClientManagerHandle.DelConn(loginBody.UserId)                               //删除tcp链接
	GlobalCache.GlobalUserLoginStatus.DelOnlineUser(loginBody.UserId)           //在线人员列表删除
	NoticeAllOnlineUserChangeStatus(loginBody.UserId, PackManager.LogoffStatus) //通知所有人退出
	return pack
}
func UploadUserHeadImage(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	GlobalCache.InitFileStream(taskInfo.UserId, taskInfo.FileName)
	resPack := PackManager.Pack{}
	resPack.Header.Method = pack.Header.Method
	resPack.Header.MethodType = 0
	resPack.Body = pack.Body
	data := createSendBuffer(resPack)
	client.conn.Write(data)
	return pack
}
func UploadUserHeadImageData(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	taskInfo := FileMethods.TaskInfo{}
	if err := json.Unmarshal(pack.Body[:pack.Header.MethodType], &taskInfo); err != nil {
		LogService.Logger.Error("UploadUserHeadImage 解析json数据失败")
		return
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(taskInfo.UserId)
	if !status {
		return nil
	}
	f := GlobalCache.GetUserFileStream(taskInfo.UserId)
	taskInfo.FileData = pack.Body[pack.Header.MethodType:]
	if f != nil {
		f.Seek(int64(taskInfo.FileCurrentSeek), 0)
		nLen, err := f.Write(taskInfo.FileData)
		len1 := int64(nLen)
		if err != nil || len1 != taskInfo.FileCurrentSize {
			LogService.Logger.Error("head image file Write error")
			return
		}
	}
	pack.Header.Method = PackManager.UploadUserHeadImageData
	taskInfo.FileCurrentSeek = taskInfo.FileCurrentSeek + taskInfo.FileCurrentSize
	if taskInfo.FileCurrentSeek == taskInfo.FileSize {
		taskInfo.FilePause = true
		GlobalCache.ClearFileStream(taskInfo.UserId)
		files := strings.Split(taskInfo.FileName, ".")
		filePath := "./headImage/" + strconv.Itoa(taskInfo.UserId) + "." + files[len(files)-1]
		MysqlManager.UpdateUserHeadImagePath(taskInfo.UserId, filePath)
		GlobalCache.GlobalUsersInfoCache.UpdateUserInfo(taskInfo.UserId)
	}
	pack.Body, _ = json.Marshal(taskInfo)
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}
func sendImage2User(userId int, userName1 string, client *TcpClient) {
	_, userInfo := GlobalCache.GlobalUsersInfoCache.GetInfoById(userId)
	// 指定要读取的文件路径
	filePath := userInfo.HeadImagePath // 将此处的文件路径替换为您要读取的文件的实际路径

	// 打开文件
	fileInfo, err := os.Stat(filePath)
	if err != nil {
		LogService.Logger.Error("无法打开文件: " + err.Error())

	} else {
		file, _ := os.Open(filePath)
		fileSize := fileInfo.Size()
		defer file.Close()

		// 创建一个缓冲区来存储数据块
		buffer := make([]byte, 8192)
		index := int64(0)
		// 无限循环，直到文件末尾
		for {
			// 从文件中读取一个数据块
			n, err := file.Read(buffer)
			if err != nil {
				if err == io.EOF {
					// 已到达文件末尾，退出循环
					break
				} else {
					log.Fatalf("读取文件时出错: %v", err)
				}
			}
			// 处理读取的数据块，这里可以根据需要进行处理
			sendPack := PackManager.HeadImage{}
			sendPack.FileData = buffer[:n]
			sendPack.FileSize = fileSize
			sendPack.FileCurrentSeek = index
			sendPack.FileCurrentSize = int64(n)
			sendPack.UserId = userId
			sendPack.FileName = strconv.Itoa(userId) + ".jpg"
			res := createPack(sendPack)
			_, err = client.conn.Write(res)
			if err != nil {
				LogService.Logger.Error(err.Error())
				return
			}
			index = index + int64(n)
		}
	}

}
func createPack(data PackManager.HeadImage) []byte {
	pack := PackManager.Pack{}
	//设置一下包头
	pack.Header.Method = PackManager.GetUserHeadImageData
	marshal, _ := json.Marshal(data)
	pack.Header.MethodType = len(marshal)
	//合并两个切片设置到body
	pack.Body = marshal
	return createHeadSendBuffer(pack, data.FileData)
}
func createHeadSendBuffer(pack PackManager.Pack, fileData []byte) []byte {
	var buffer bytes.Buffer //Buffer是一个实现了读写方法的可变大小的字节缓冲
	buffer.Write(intToBytes(PackManager.HeaderSize + len(pack.Body) + len(fileData)))
	buffer.Write(intToBytes(pack.Header.Method))
	buffer.Write(intToBytes(pack.Header.MethodType))
	buffer.Write(pack.Body)
	buffer.Write(fileData)
	return buffer.Bytes()
}
