package MysqlManager

import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"testserver/LogService"
	"testserver/settings"
)

//存储系统数据 人员账号密码，系统注册信息，群组信息等

func StartMysqlService(cfg *settings.MysqlConfig) {

	connectStr := fmt.Sprintf("%s:%s@tcp(%s:%d)/%s?charset=utf8",
		cfg.User,
		cfg.Password,
		cfg.Host,
		cfg.Port,
		cfg.DbName,
	)

	pool, err := sql.Open("mysql", connectStr)
	if err != nil && pool != nil {
		LogService.Logger.Error("database init error = " + err.Error())
		return
	}
	LogService.Logger.Info("Mysql 连接成功")
	dbPool = pool
	dbPool.SetMaxOpenConns(cfg.MaxOpenConns)
	dbPool.SetMaxIdleConns(cfg.MaxIdleConns)
	databaseInit()
	MysqlChannel = make(chan string, 100) //带缓存的 channel
	go queryInsert()                      //插入采用异步执行，其他同步执行
}

func databaseInit() {

}

// 需要直接返回的
func InsertLht(sql string) error {
	rows, _ := dbPool.Query(sql)
	if rows != nil {
		//err := rows.Close()
		//if err != nil {
		//	return err
		//} //释放连接
	}
	return nil
}

func GetRows(sql string) *sql.Rows {
	rows, _ := dbPool.Query(sql)
	return rows
}

// Insert 外部调用函数
func Insert(sql string) {
	MysqlChannel <- sql
}

func queryInsert() {
	for {
		select {
		case sqlStr := <-MysqlChannel:
			go insert(sqlStr)
		}
	}
}

func insert(sql string) {
	rows, _ := dbPool.Query(sql)
	if rows != nil {
		err := rows.Close()
		if err != nil {
			return
		} //释放连接
	}

}

func checkErr(err error) {

	if err != nil {

		LogService.Logger.Error(err.Error())
	}
}
