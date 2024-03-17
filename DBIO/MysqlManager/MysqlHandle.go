package MysqlManager

import (
	"errors"
	"fmt"
	"testserver/LogService"
	"testserver/PackManager"
)

func QuerySql(str string) bool {
	_, err := dbPool.Query(str)
	if err != nil {
		LogService.Logger.Error(err.Error())
		return false
	}
	return true
}

// SelectUsers 获取所有人员信息
func SelectUsers(user map[int]PackManager.GlobalUserInfo) error {
	rows, _ := dbPool.Query(SelectUsersQuery)
	//user := make(map[int]share.GlobalUserInfo)
	if rows == nil {
		LogService.Logger.Error("users table is nil")
		return errors.New("users table is nil")
	}
	for rows.Next() {
		userData := PackManager.GlobalUserInfo{}
		err := rows.Scan(&userData.UserId, &userData.UserName, &userData.LoginName,
			&userData.Password, &userData.LastLogin, &userData.DeptId,
			&userData.PermissionLevel, &userData.CreateTime, &userData.CreateId,
			&userData.PhoneNumber, &userData.Email, &userData.HeadImagePath)
		user[userData.UserId] = userData
		checkErr(err)
	}
	err := rows.Close()
	if err != nil {
		return errors.New("rows.Close error")
	} //释放连接
	return nil
}
func UpdateUserHeadImagePath(userId int, path string) {
	query := fmt.Sprintf(UpdateUserHeadImagePathQuery, path, userId)

	_, err := dbPool.Query(query)
	if err != nil {
		LogService.Logger.Error("UpdateUserHeadImagePath: " + err.Error())
		return
	}
}

func GetUserInfo(userId int, userData *PackManager.GlobalUserInfo) {
	query := fmt.Sprintf(SelectUserQuery, userId)
	rows, _ := dbPool.Query(query)
	//user := make(map[int]share.GlobalUserInfo)
	if rows == nil {
		LogService.Logger.Error("users table is nil")
		return
	}
	for rows.Next() {

		err := rows.Scan(&userData.UserId, &userData.UserName, &userData.LoginName,
			&userData.Password, &userData.LastLogin, &userData.DeptId,
			&userData.PermissionLevel, &userData.CreateTime, &userData.CreateId,
			&userData.PhoneNumber, &userData.Email, &userData.HeadImagePath)
		checkErr(err)
	}
	err := rows.Close()
	if err != nil {
		return
	} //释放连接
}

// SelectOrgs 获取所有部门的信息
func SelectOrgs() map[int]PackManager.DeptItem {
	rows, _ := dbPool.Query(SelectAllDeptInfo)
	depths := make(map[int]PackManager.DeptItem)
	if rows == nil {
		LogService.Logger.Error("dept_info table is nil")
		return nil
	}
	for rows.Next() {
		dept := PackManager.DeptItem{}
		err := rows.Scan(&dept.DeptName, &dept.DeptID, &dept.ParentDeptID, &dept.Level)
		depths[dept.DeptID] = dept
		checkErr(err)
	}
	err := rows.Close()
	if err != nil {
		return nil
	} //释放连接
	return depths
}

// SelectUserCache 查询
func SelectUserCache(sql string) map[string]UserData {
	rows, _ := dbPool.Query(sql)
	user := make(map[string]UserData)
	if rows == nil {
		LogService.Logger.Error("userinfo table is nil")
		return nil
	}
	for rows.Next() {
		userData := UserData{}
		err := rows.Scan(&userData.id, &userData.UserName, &userData.UserLoginName,
			&userData.PassWord, &userData.Notice, &userData.MacAddress,
			&userData.LoginTime, &userData.ParentDeptID)
		user[userData.UserLoginName] = userData
		checkErr(err)
	}
	err := rows.Close()
	if err != nil {
		return nil
	} //释放连接
	return user
}

// SelectOrg 查询
func SelectOrg(sql string) map[int]DeptData {
	rows, _ := dbPool.Query(sql)
	depths := make(map[int]DeptData)
	if rows == nil {
		LogService.Logger.Error("dept_info table is nil")
		return nil
	}
	for rows.Next() {
		dept := DeptData{}
		err := rows.Scan(&dept.DeptName, &dept.DeptID, &dept.ParentDeptID, &dept.Level)
		depths[dept.DeptID] = dept
		checkErr(err)
	}
	err := rows.Close()
	if err != nil {
		return nil
	} //释放连接
	return depths
}
