package WebApis

import (
	"crypto/rand"
	"encoding/base64"
	"github.com/gin-gonic/gin"
	"strconv"
	"testserver/PackManager"
	"testserver/logger"
	"time"
)

// UserService 定义用户相关操作的接口
type UserService interface {
	Login(data *LoginData) (bool, error)
	AddUser(data *AddUserData) (bool, error)
}

//var cache *UserCache.UserCache //= UserCache.NewUserCache()

// App 结构体持有全局引擎
type App struct {
	Engine       *gin.Engine
	tokenManager *TokenManager
}

// NewApp 使用给定的地址和端口初始化 App 结构体
func NewApp(address string, port int) *App {
	app := &App{
		Engine: gin.New(),
	}
	app.Engine.Use(logger.GinLogger(), logger.GinRecovery(true))
	app.Engine.Use(logger.Cors())
	app.initRoutes()
	app.tokenManager = NewTokenManager()
	//cache = UserCache.NewUserCache()
	go app.Engine.Run(address + ":" + strconv.Itoa(port))

	return app
}

// initRoutes 初始化路由和处理程序
func (app *App) initRoutes() {
	app.Engine.POST("/api/login", app.handleLogin)
	app.Engine.GET("/api/getMenuList", app.handleGetMenuList)

	app.Engine.POST("/api/getUserList", app.handleGetUserList)
	app.Engine.POST("/api/getGroupList", app.handleGetGroupList)
	app.Engine.POST("/api/getOrg", app.handleGetOrg)
	app.Engine.POST("/api/updateOrg", app.handleUpdateOrg)
	app.Engine.POST("/api/addUser", app.handleAddUser)
	app.Engine.POST("/api/modifyUser", app.handleModifyUser)
	app.Engine.POST("/api/deleteUser", app.handleDeleteUser)
	app.Engine.POST("/api/addGroup", app.handleAddGroup)
	app.Engine.POST("/api/modifyGroup", app.handleModifyGroup)
	app.Engine.POST("/api/deleteGroup", app.handleDeleteGroup)
}

func (app *App) generateRandomToken() (token string) {
	tokenBytes := make([]byte, 8)
	_, err := rand.Read(tokenBytes)
	if err != nil {
		return ""
	}
	return base64.URLEncoding.EncodeToString(tokenBytes)[:8]
}

func (app *App) addToken(info PackManager.GlobalUserInfo) (token string) {
	return app.tokenManager.GenerateToken(10*time.Minute, info)
}

func (app *App) delToken(token string) {
	app.tokenManager.DelToken(token)
}

func (app *App) checkTokenStatus(token string) bool {
	if ok := app.tokenManager.IsTokenValid(token); ok {
		return true
	}
	return false
}
