package WebApis

/**
*	这个文件主要为网页请求进入后的json处理
 */
import (
	"github.com/gin-gonic/gin"
	"net/http"
	"testserver/GlobalCache"
	"testserver/LogService"
)

func (app *App) handleLogin(c *gin.Context) {
	loginData := LoginData{
		Username: "",
		Password: "",
	}
	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	isValid, err, info := app.Login(&loginData)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error(), "code": http.StatusInternalServerError})
		return
	}

	if isValid {
		token := app.addToken(info)
		c.JSON(http.StatusOK, gin.H{"data": gin.H{
			"message":   "登录成功",
			"userId":    info.UserId,
			"userName":  info.UserName,
			"userEmail": "niudingding@niu.com",
			"token":     token,
			"state":     1,
			"deptId":    info.DeptId,
			"role":      info.PermissionLevel,
		},
			"msg": "", "code": http.StatusOK,
		})
	} else {
		c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证", "code": 304})
	}
}

func (app *App) handleGetMenuList(c *gin.Context) {
	query := c.Request.URL.Query()
	var queryMap = make(map[string]string, len(query))
	for k := range query {
		queryMap[k] = c.Query(k)
	}
	token := queryMap["token"]

	if !app.checkTokenStatus(token) {
		c.JSON(http.StatusBadRequest, gin.H{"error": "令牌失效"})
		LogService.Logger.Error("token 失效")
		return
	}
	listByte := app.GetMenuList()
	c.JSON(http.StatusOK, gin.H{
		"data": listByte,
		"code": http.StatusOK,
	})
}

// update user
func (app *App) handleAddUser(c *gin.Context) {
	addUserData := AddUserData{
		Token:      "",
		UserName:   "",
		OrgId:      0,
		LoginName:  "",
		LoginPwd:   "",
		UserNumber: "",
	}
	if err := c.ShouldBindJSON(&addUserData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if !app.checkTokenStatus(addUserData.Token) {
		c.JSON(http.StatusBadRequest, gin.H{"error": "令牌失效"})
		return
	}
	isValid, err := app.AddUser(&addUserData)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	if isValid {
		c.JSON(http.StatusOK, gin.H{"message": "添加成功"})
	} else {
		c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	}
}

func (app *App) handleModifyUser(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	//	c.JSON(http.StatusOK, gin.H{"message": "登录成功"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func (app *App) handleDeleteUser(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	//
	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	//	c.JSON(http.StatusOK, gin.H{"message": "登录成功"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

// update group
func (app *App) handleAddGroup(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func (app *App) handleModifyGroup(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func (app *App) handleDeleteGroup(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func (app *App) handleGetUserList(c *gin.Context) {
	reqStruct := GetDataStruct{}
	if err := c.ShouldBindJSON(&reqStruct); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	if !app.checkTokenStatus(reqStruct.Token) {
		c.JSON(http.StatusBadRequest, gin.H{"error": "令牌失效"})
		return
	}

	userJson := GlobalCache.GlobalUsersInfoCache.GetAllUserJson()
	// 设置 JSON 响应头并返回 JSON 数据
	c.Header("Content-Type", "application/json")
	c.JSON(http.StatusOK, gin.H{"data": string(userJson)})

}

func (app *App) handleGetGroupList(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func (app *App) handleGetOrg(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}
func (app *App) handleUpdateOrg(c *gin.Context) {
	var loginData struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}

	if err := c.ShouldBindJSON(&loginData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	//isValid, err := app.Login(loginData.Username, loginData.Password)
	//if err != nil {
	//	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	//	return
	//}
	//
	//if isValid {
	c.JSON(http.StatusOK, gin.H{"message": "success"})
	//} else {
	//	c.JSON(http.StatusUnauthorized, gin.H{"message": "无效的凭证"})
	//}
}

func checkToken(c *gin.Context, token string) (checkState bool) {
	c.JSON(http.StatusBadRequest, gin.H{"error": "无效的凭证"})
	return false
}
