package GlobalCache

import (
	"fmt"
	"log"
	"testserver/DBIO/MysqlManager"
	"testserver/PackManager"
)

var GlobalGroupCache *GroupCache

func NewGroupCache() {
	GlobalGroupCache = &GroupCache{
		Groups: make(map[string][]PackManager.GroupUsers),
	}
	GlobalGroupCache.InitGroup()
}
func (groupCache *GroupCache) InitGroup() {
	rows := MysqlManager.GetRows(MysqlManager.GetAllGroupUsersQuery)
	defer rows.Close()
	for rows.Next() {
		var g PackManager.GroupUsers
		if err := rows.Scan(&g.Id, &g.GroupID, &g.UserId, &g.JoinAt, &g.UserLevel); err != nil {
			log.Fatal(err)
		}
		groupCache.Groups[g.GroupID] = append(groupCache.Groups[g.GroupID], g)
	}
	if err := rows.Err(); err != nil {
		log.Fatal(err)
	}
}

func (groupCache *GroupCache) UpdateGroup(groupId string) {
	queryStr := fmt.Sprintf(MysqlManager.GetGroupUsersQuery, groupId)
	rows := MysqlManager.GetRows(queryStr)
	defer rows.Close()
	delete(groupCache.Groups, groupId)
	for rows.Next() {
		var g PackManager.GroupUsers
		if err := rows.Scan(&g.Id, &g.GroupID, &g.UserId, &g.JoinAt, &g.UserLevel); err != nil {
			log.Fatal(err)
		}
		groupCache.Groups[groupId] = append(groupCache.Groups[g.GroupID], g)
	}
	if err := rows.Err(); err != nil {
		log.Fatal(err)
	}
}
