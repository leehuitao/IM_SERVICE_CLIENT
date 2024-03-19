package Utils

import "github.com/google/uuid"

func CreateUuid() string {
	id := uuid.New()
	return id.String()
}
