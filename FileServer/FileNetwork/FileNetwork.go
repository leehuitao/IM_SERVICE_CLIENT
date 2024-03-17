package FileNetwork

import (
	"net"
	"sync"
	"testserver/FileServer/FileMethods"
	"testserver/LogService"
)

var numberMutex sync.Mutex

type TcpServer struct {
	//当前登录的个数
	currentClientNumber int
	//历史登录个数
	historyClientNumber int
	//监听socket
	server net.Listener
}

// UpdateCurrentClientNumber 更新当前登录的个数
func (server *TcpServer) UpdateCurrentClientNumber(updateNumber int) {
	numberMutex.Lock()
	defer numberMutex.Unlock()
	server.currentClientNumber = server.currentClientNumber + updateNumber

}

// GetCurrentClientNumber 获取当前登录的个数
func (server *TcpServer) GetCurrentClientNumber() (currentClientNumber int) {
	return server.currentClientNumber
}

// AddHistoryClientNumber 新用户登录时增加一个
func (server *TcpServer) AddHistoryClientNumber() {
	server.historyClientNumber = server.historyClientNumber + 1
}

// GetHistoryClientNumber 获取历史登录个数
func (server *TcpServer) GetHistoryClientNumber() (historyClientNumber int) {
	return server.historyClientNumber
}

// StartListen 服务开始监听 address (127.0.0.1:8000)
func (server *TcpServer) StartListen(address string) {

	listen, err := net.Listen("tcp", address)
	server.server = listen
	if err != nil {
		LogService.Logger.Error("start listen error " + err.Error())
		return
	}
	LogService.Logger.Info("网络服务启动 address = " + address)
	defer server.server.Close()
	for {
		// 监听客户端连接请求

		conn, err := server.server.Accept()
		if err != nil {
			LogService.Logger.Error("listener.Accept err" + err.Error())
			return
		}
		// 设置一下连接的参数
		var client FileMethods.FileClient
		FileMethods.AddClient(client)
		client.UpdateConn(conn)
		// 开始获取数据
		go client.StartRead(conn)
	}
}

// StopListen 服务停止监听
func (server *TcpServer) StopListen() {
	err := server.server.Close()
	if err != nil {
		return
	}
}
