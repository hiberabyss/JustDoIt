package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func connHandler(c net.Conn) {
	defer c.Close()

	reader := bufio.NewReader(os.Stdin)
	buf := make([]byte, 1024)

	for {
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)

		if input == "quit" {
			return
		}

		c.Write([]byte(input))

		cnt, err := c.Read(buf)
		if err != nil {
			fmt.Printf("Fail to read data, %s\n", err)
			continue
		}

		fmt.Print(string(buf[0:cnt]))
	}
}

func main() {
	conn, err := net.Dial("tcp", "localhost:1208")
	if err != nil {
		fmt.Printf("Fail to connect, %s\n", err)
		return
	}

	connHandler(conn)
}
