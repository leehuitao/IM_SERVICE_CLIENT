package WebApis

import (
	"crypto/rand"
	"encoding/base64"
	"sync"
	"testserver/PackManager"
	"time"
)

type LoginData struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

type GetDataStruct struct {
	Token string `json:"token"`
}

type AddUserData struct {
	Token      string `json:"token"`
	UserName   string `json:"username"`
	OrgId      int    `json:"orgId"`
	LoginName  string `json:"loginName"`
	LoginPwd   string `json:"loginPwd"`
	UserNumber string `json:"userNumber"`
}

type Token struct {
	Value     string
	UserInfo  PackManager.GlobalUserInfo
	ExpiresAt time.Time
}

type TokenManager struct {
	tokens map[string]Token
	mutex  sync.Mutex
}

func NewTokenManager() *TokenManager {
	return &TokenManager{
		tokens: make(map[string]Token),
	}
}

func (tm *TokenManager) GenerateToken(duration time.Duration, pack PackManager.GlobalUserInfo) string {
	tokenBytes := make([]byte, 16)
	_, _ = rand.Read(tokenBytes)
	token := base64.URLEncoding.EncodeToString(tokenBytes)

	expiration := time.Now().Add(duration)
	tm.mutex.Lock()
	defer tm.mutex.Unlock()
	tm.tokens[token] = Token{
		Value:     token,
		UserInfo:  pack,
		ExpiresAt: expiration,
	}

	return token
}

func (tm *TokenManager) DelToken(token string) {
	tm.mutex.Lock()
	defer tm.mutex.Unlock()

	_, found := tm.tokens[token]
	if found {
		delete(tm.tokens, token)
	}
}

func (tm *TokenManager) IsTokenValid(token string) bool {
	tm.mutex.Lock()
	defer tm.mutex.Unlock()

	tok, found := tm.tokens[token]
	if !found {
		return false
	}

	return time.Now().Before(tok.ExpiresAt)
}
