package LogService

//
//import (
//	"fmt"
//	"log"
//	"os"
//	"strconv"
//)
//
//type fileLog string
//
//var debugChannel chan string
//var errorChannel chan string
//
//func StartLogService(fileName string) {
//	debugChannel = make(chan string, 100)
//	errorChannel = make(chan string, 100)
//	go goLogDebug(fileName)
//	go goLogError(fileName)
//}
//
//func LogDebug(args ...any) {
//	var logStr string
//	for _, v := range args {
//		var str string
//		switch v.(type) {
//		case int:
//			str = strconv.Itoa(v.(int))
//		case int64:
//			str = strconv.Itoa(int(v.(int64)))
//		case string:
//			str = v.(string)
//		case []byte:
//			str = string(v.([]byte))
//		}
//		logStr = logStr + str
//		//fmt.Println(v)
//	}
//	debugChannel <- logStr
//}
//
//func LogError(args ...any) {
//	var logStr string
//	for _, v := range args {
//		var str string
//		switch v.(type) {
//		case int:
//			str = strconv.Itoa(v.(int))
//		case int64:
//			str = strconv.Itoa(int(v.(int64)))
//		case string:
//			str = v.(string)
//		case []byte:
//			str = string(v.([]byte))
//		}
//		logStr = logStr + str
//		//fmt.Println(v)
//	}
//	errorChannel <- logStr
//}
//
//func goLogDebug(fileName string) {
//	debugLogFile, err := os.OpenFile(fileName+"_debug.log", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0600)
//	if err != nil {
//		return
//	}
//	logger := log.New(debugLogFile, " debug ", log.LstdFlags)
//	defer debugLogFile.Close()
//	for {
//		select {
//		case data := <-debugChannel:
//			logger.Println(data + "")
//			fmt.Println(data)
//		}
//	}
//}
//
//func goLogError(fileName string) {
//	errorLogFile, err := os.OpenFile(fileName+"_error.log", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0600)
//	if err != nil {
//		return
//	}
//	logger := log.New(errorLogFile, " error ", log.LstdFlags)
//	defer errorLogFile.Close()
//	for {
//		select {
//		case data := <-errorChannel:
//			logger.Println(data)
//			fmt.Println(data)
//		}
//	}
//}
