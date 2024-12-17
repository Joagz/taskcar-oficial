package server

import (
	"fmt"
	"net"
	"strconv"
	"strings"
	"taskcar/data"
)

type ServerData struct {
	Host          string
	Port          int
	root_user     string
	root_password string // ? encrypt
	Running       bool
}

func NewServerData(host string, port int, root_user string, root_password string) ServerData {
	return ServerData{
		Host:          host,
		Port:          port,
		root_user:     root_user,
		root_password: data.Encrypt(root_password),
	}
}

func (srv ServerData) Address() string {
	return srv.Host + ":" + strconv.Itoa(srv.Port)
}

func InitTCP(srv ServerData, flag chan ServerData) {

	addr, err := net.ResolveTCPAddr("tcp", srv.Address())

	checkErrExit(err)

	listener, err := net.ListenTCP("tcp", addr)

	checkErrExit(err)

	fmt.Println("Server started")

	srv.Running = true

	flag <- srv

	for {
		client, err := listener.Accept()

		if err == nil {
			handler, err := handleConnection(client)

			if err != nil {
				client.Close()
				fmt.Println(err.Error())
				return
			}

			if handler == nil {
				fmt.Println("handler is nil")
				continue
			}

			for {
				buffer := make([]byte, 1024)

				bytes, err := client.Read(buffer)

				array := strings.Split(string(buffer), "\n")

				for _, buf := range array {

					if err != nil {
						continue
					}

					if bytes == 0 {
						fmt.Println("no data read")
						continue
					}

					obj, _ := handler.Deserialize([]byte(buf))

					if obj != nil {
						handler.Callback(obj)
					}
				}
			}
		}

	}
}
