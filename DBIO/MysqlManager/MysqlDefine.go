package MysqlManager

import "database/sql"

var dbPool *sql.DB

var MysqlChannel chan string

type UserData struct {
	id            int    `db:"id"`
	UserName      string `db:"UserName"`
	UserLoginName string `db:"UserLoginName"`
	PassWord      string `db:"PassWord"`
	Notice        int    `db:"Notice"`
	MacAddress    string `db:"MacAddress"`
	LoginTime     string `db:"LoginTime"`
	ParentDeptID  int    `db:"ParentDeptID"`
}

type DeptData struct {
	DeptName     string `db:"DeptName"`
	DeptID       int    `db:"DeptID"`
	ParentDeptID int    `db:"ParentDeptID"`
	Level        int    `db:"Level"`
}

const (
	SelectUsersQuery = "SELECT id,username,login_name,password,last_login,dept_id,permission_level," +
		"created_time,creator_id,phone_number,email,head_path from users"

	SelectUserQuery = "SELECT id,username,login_name,password,last_login,dept_id,permission_level," +
		"created_time,creator_id,phone_number,email,head_path from users where id = %d"
	SelectAllDeptInfo = "select * from dept_info"
	InsertLoginRecord = "INSERT INTO login_logs ( `user_id`, `user_name`, `login_time`, `ip_address`) " +
		"VALUES (%d, '%s', '%s', '%s');"
	InsertMessageRecord = "INSERT INTO personal_chat_messages (`msg_id`,`sender_id`, `sender_name`, `receiver_id`, `receiver_name`, `content`, `send_time`) " +
		"VALUES ( '%s',%d, '%s', %d, '%s', '%s', '%s');"
	UpdateUserHeadImagePathQuery = "UPDATE users SET head_path = '%s' WHERE id = %d"

	CreateNewGroupQuery = "INSERT INTO groups (`group_id`, `group_name`, `announcement`, `created_at`, `creator_id`)" +
		"VALUES ('%s', '%s', '%s', '%s',%d)"

	AddGroupMemberQuery      = "INSERT INTO group_members (`group_id`, `user_id`,`user_level`) VALUES ('%s', %d,%d)"
	GetAllGroupUsersQuery    = "SELECT * FROM group_members"
	GetGroupsQuery           = "SELECT g.* FROM group_members gm JOIN groups g ON gm.group_id = g.group_id WHERE gm.user_id = %d"
	GetGroupUsersQuery       = "SELECT * FROM group_members  WHERE group_id = '%s'"
	InsertMessageGroupRecord = "INSERT INTO group_messages (`msg_id`, `group_id`, `group_name`, `user_id`, `user_name`, `content`) VALUES ('%s', '%s', '%s', %d, '%s', '%s')"
)
