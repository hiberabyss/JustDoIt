package main

import (
	"fmt"
	"io"
	"net/http"
	"sync"
	"time"
)

var QPS []CountQPS

type CountQPS struct {
	CountPerSecond int
	Timestamp      int64
}

type CounterService struct {
	CountQPS
	CountAll       int
	Lock           sync.Mutex
}

func NewCounterService() *CounterService {
	counter := &CounterService{}
	go func() {
		ticker := time.NewTicker(time.Second)
		for {
			<-ticker.C
			counter.Lock.Lock()
			counter.Timestamp = time.Now().Unix()

			if counter.CountPerSecond > 0 {
				QPS = append(QPS, CountQPS{counter.CountPerSecond, counter.Timestamp})
			}

			counter.CountPerSecond = 0

			counter.Lock.Unlock()
		}
	}()
	return counter
}

func (counter *CounterService) Increase() {
	counter.Lock.Lock()
	defer counter.Lock.Unlock()

	counter.CountAll++
	counter.CountPerSecond++
}

func getCntHandler(w http.ResponseWriter, r *http.Request) {
	cntStr := "timestamp,query_per_second\n"

	for _, c := range QPS {
		cntStr += fmt.Sprintf("%d,%d\n", c.Timestamp, c.CountPerSecond)
	}

	cntStr += fmt.Sprintf("total: %d\n", Counter.CountAll)

	io.WriteString(w, cntStr)
}

func helloHandler(w http.ResponseWriter, r *http.Request) {
	Counter.Increase()
	io.WriteString(w, "Hello world!\n")
}

var Counter = NewCounterService()

func main() {
	fmt.Println("Server started!")
	http.HandleFunc("/", helloHandler)
	http.HandleFunc("/get_cnt", getCntHandler)
	http.ListenAndServe(":8000", nil)
}
