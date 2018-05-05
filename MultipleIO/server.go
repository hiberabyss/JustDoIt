package main

import (
	"fmt"
	"net"
)

func connHandler(c net.Conn) {
	buf := make([]byte, 1024)
	defer c.Close()

	cnt, _ := c.Read(buf)
	c.Write(buf[0:cnt])
}

func main() {
	server, _ := net.Listen("tcp", ":1208")

	fmt.Println("Server started ...")

	for {
		conn, _ := server.Accept()
		
		go connHandler(conn)
	}
}
