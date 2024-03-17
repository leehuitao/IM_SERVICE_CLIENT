// Package GlobalCache 全部部门方法
package GlobalCache

import (
	"encoding/json"
	"testserver/DBIO/MysqlManager"
	"testserver/PackManager"
)

var GlobalDeptCache DeptCache

func (org *DeptCache) AddDept(data PackManager.DeptItem) {
	org.dept = append(org.dept, data)
	b, _ := json.Marshal(data)
	org.deptJson[data.DeptID] = b
}

func (org *DeptCache) RemoveDept(deptId int) {
	// 删除切片中的项
	for i, item := range org.dept {
		if item.DeptID == deptId {
			org.dept = append(org.dept[:i], org.dept[i+1:]...)
			break
		}
	}

	// 删除映射中的项
	delete(org.deptJson, deptId)
}
func (org *DeptCache) GetJson() []byte {
	marshal, err := json.Marshal(org.deptJson)
	if err != nil {
		return nil
	}
	return marshal
}

func newOrgCache(orgCache *DeptCache) {
	orgCache.dept = []PackManager.DeptItem{}
	orgCache.deptJson = make(map[int]interface{})

}

func OrgInit() {
	//重置组织架构
	newOrgCache(&GlobalDeptCache)
	org := MysqlManager.SelectOrgs()
	for _, v := range org {
		item := PackManager.DeptItem{}
		item.DeptName = v.DeptName
		item.DeptID = v.DeptID
		item.ParentDeptID = v.ParentDeptID
		item.Level = v.Level
		GlobalDeptCache.AddDept(item)
	}
}
