package Methods

import (
	"encoding/json"
	"strconv"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
)

func UpdateStatus(pack *PackManager.Pack, client *TcpClient) (requestPack *PackManager.Pack) {
	loginBody := PackManager.LoginBody{}
	if err := json.Unmarshal(pack.Body, &loginBody); err != nil {
		return nil
	}
	status := GlobalCache.GlobalUserLoginStatus.CheckUserIsOnline(loginBody.UserId)
	if !status {
		LogService.Logger.Error(getCurrentFunctionName() + "-->" + "用户ID验证失败：" + strconv.Itoa(loginBody.UserId))
		return nil
	}
	resPack := PackManager.Pack{}
	resPack.Header.Method = pack.Header.Method
	resPack.Header.MethodType = 0
	resPack.Body = []byte{}
	data := createSendBuffer(resPack)
	client.conn.Write(data)
	GlobalCache.GlobalUserLoginStatus.UpdateOnlineUser(loginBody.UserId, loginBody.LoginStatus)
	//userCache.UpdateUserLoginStatus(loginBody.UserLoginName, loginBody.LoginStatus)
	NoticeAllOnlineUserChangeStatus(loginBody.UserId, loginBody.LoginStatus)
	return pack
}
