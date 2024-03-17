package Methods

import (
	"encoding/json"
	"strconv"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/settings"
)

func GetFileServerAddr(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	sysBody := PackManager.SystemBody{}
	if err := json.Unmarshal(pack.Body, &sysBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(sysBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(sysBody.UserId))
		return nil
	}
	addr := settings.Conf.FileServerConfig.Host + ":" + strconv.Itoa(settings.Conf.FileServerConfig.Port)
	pack.Body = []byte(addr)
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}

func UpdateConnectFileServerState(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	fileBody := PackManager.FileBody{}
	if err := json.Unmarshal(pack.Body, &fileBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(fileBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(fileBody.UserId))
		return nil
	}
	pack.Body, _ = json.Marshal(fileBody)
	if fileBody.UserId == fileBody.SendUserId { //发送者完成连接
		getConn := ClientManagerHandle.GetConn(fileBody.RecvUserId)
		if getConn.conn != nil {
			data := createSendBuffer(*pack)
			getConn.conn.Write(data)
		}
	} else if fileBody.UserId == fileBody.RecvUserId { //接收者完成连接
		pack.Header.Method = PackManager.ConnectFileServerSuccess
		getConn := ClientManagerHandle.GetConn(fileBody.SendUserId)
		if getConn.conn != nil {
			data := createSendBuffer(*pack)
			getConn.conn.Write(data)
		}
	} else {
		client.conn.Close()
	}
	data := createSendBuffer(*pack)
	client.conn.Write(data)
	return pack
}
