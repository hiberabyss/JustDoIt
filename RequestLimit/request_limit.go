package main

import (
	"fmt"
	"io"
	"net/http"
	"sync"
	"time"
)

type RequestLimitService struct {
	Interval time.Duration
	MaxCount int
	Lock     sync.Mutex
	ReqCount int
}

func NewRequestLimitService(interval time.Duration, maxCnt int) *RequestLimitService {
	reqLimit := &RequestLimitService{
		Interval: interval,
		MaxCount: maxCnt,
	}

	go func() {
		ticker := time.NewTicker(interval)
		for {
			<-ticker.C
			reqLimit.Lock.Lock()
			fmt.Println("Reset Count...")
			reqLimit.ReqCount = 0
			reqLimit.Lock.Unlock()
		}
	}()

	return reqLimit
}

func (reqLimit *RequestLimitService) Increase() {
	reqLimit.Lock.Lock()
	defer reqLimit.Lock.Unlock()

	reqLimit.ReqCount += 1
}

func (reqLimit *RequestLimitService) IsAvailable() bool {
	reqLimit.Lock.Lock()
	defer reqLimit.Lock.Unlock()

	return reqLimit.ReqCount < reqLimit.MaxCount
}

var RequestLimit = NewRequestLimitService(10 * time.Second, 5)

func helloHandler(w http.ResponseWriter, r *http.Request) {
	if RequestLimit.IsAvailable() {
		RequestLimit.Increase()
		fmt.Println(RequestLimit.ReqCount)
		io.WriteString(w, "Hello world!\n")
	} else {
		fmt.Println("Reach request limiting!")
		io.WriteString(w, "Reach request limit!\n")
	}
}

func main() {
	fmt.Println("Server Started!")
	http.HandleFunc("/", helloHandler)
	http.ListenAndServe(":8000", nil)
}
