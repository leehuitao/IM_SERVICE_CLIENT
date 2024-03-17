package WebApis

import (
	"encoding/json"
	"fmt"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
	"testserver/GlobalCache"
	"testserver/LogService"
	"testserver/PackManager"
	"testserver/settings"
)

/**
*	这个文件主要为网页数据进行内存操作时的函数 为webHandler的调用函数的地方
 */

// Login 在 App 结构体中实现 UserService 接口的方法
func (app *App) Login(data *LoginData) (bool, error, PackManager.GlobalUserInfo) {
	//userData := UserCache.OrgCacheData
	err, password := GlobalCache.GlobalUsersInfoCache.GetUserPwdByName(data.Username)
	ret := PackManager.GlobalUserInfo{}

	if err != nil {
		LogService.Logger.Error("不存在当前用户")
		return false, err, ret
	}

	if password == data.Password {
		_, p := GlobalCache.GlobalUsersInfoCache.GetInfoByName(data.Username)

		return true, nil, p
	}

	return false, nil, ret
}

func (app *App) AddUser(data *AddUserData) (bool, error) {
	if true {
		return true, nil
	}
	return false, nil
}

// 生成数据
type MenuList struct {
	ParentId   int        `json:"ParentId"`
	UpdateTime string     `json:"UpdateTime"`
	CreateTime string     `json:"CreateTime"`
	Id         int        `json:"Id"`
	MenuType   int        `json:"MenuType"`
	MenuName   string     `json:"MenuName"`
	MenuCode   int        `json:"MenuCode"`
	Path       string     `json:"Path"`
	Icon       string     `json:"Icon"`
	Order      int        `json:"Order"`
	Component  string     `json:"Component"`
	Children   []MenuList `json:"Children" gorm:"-"`
}

// TableName MenuList
func (v MenuList) TableName() string {
	return "menu_list"
}
func (app *App) GetMenuList() []byte {

	conf := settings.Conf.MysqlConfig
	//配置MySQL连接参数
	username := conf.User     //账号
	password := conf.Password //密码
	host := conf.Host         //数据库地址，可以是Ip或者域名
	port := conf.Port         //数据库端口
	Dbname := conf.DbName     //数据库名
	timeout := "10s"          //连接超时，10秒

	//拼接下dsn参数, dsn格式可以参考上面的语法，这里使用Sprintf动态拼接dsn参数，因为一般数据库连接参数，我们都是保存在配置文件里面，需要从配置文件加载参数，然后拼接dsn。
	dsn := fmt.Sprintf("%s:%s@tcp(%s:%d)/%s?charset=utf8&parseTime=True&loc=Local&timeout=%s", username, password, host, port, Dbname, timeout)
	//连接MYSQL, 获得DB类型实例，用于后面的数据库读写操作。
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		LogService.Logger.Error("连接数据库失败, error=" + err.Error())
	}

	//延时关闭数据库连接
	//defer db.Close()

	//因为Find返回的是数组，所以定义一个商品数组用来接收结果
	var menuLists []MenuList
	var menuRetLists []MenuList
	var menuListTree map[int]MenuList = make(map[int]MenuList)
	//等价于：SELECT * FROM `foods`
	db.Order("Id asc").Find(&menuLists)
	//LogService.Logger.Info()
	//json.Marshal(menuList)
	if len(menuLists) > 0 {
		for _, v := range menuLists {
			if v.ParentId == 0 {
				menuListTree[v.Id] = v
				continue
			}
			list1 := menuListTree[v.ParentId]
			list1.Children = append(list1.Children, v)
			menuListTree[v.ParentId] = list1
		}
		for _, v := range menuListTree {
			menuRetLists = append(menuRetLists, v)
		}
		marshal, _ := json.MarshalIndent(menuRetLists, "", "  ")
		return marshal
	}
	LogService.Logger.Error("服务器生成菜单列表失败")
	return []byte{}
}
