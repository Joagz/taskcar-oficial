package server

import (
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"taskcar/config"
	"taskcar/data"
)

type ServerData struct {
	Host          string
	Port          int
	root_user     string
	root_password string // ? encrypt
	Running       bool
	flag          chan ServerData
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

func Start(host, username, password string, port int) ServerData {
	srv := NewServerData(host, port, username, password)
	flag := make(chan ServerData)
	srv.flag = flag

	go InitTCP(srv, flag)
	srv = <-flag

	if !srv.Running {
		os.Exit(-1)
	}

	return srv
}

func (srv ServerData) Wait() {
	<-srv.flag
}

func resizeBuffer(buffer []byte, lastBufferIndex int) []byte {
	return append(buffer[:lastBufferIndex], make([]byte, config.SERVER_PACKET_SIZE_BYTES)...)
}

func dispatchToHandler(buf []byte, handler *data.HandlerData) {
	obj, _ := handler.Deserialize([]byte(buf))
	fmt.Println("handler dispatched")

	if obj != nil {
		handler.Callback(obj)
		return
	}

	fmt.Println("object couldn't be deserialized")
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

			buffer := make([]byte, config.SERVER_BUFFER_SIZE_BYTES)
			lastBufferIndex := 0

			for {
				if cap(buffer)-config.SERVER_PACKET_SIZE_BYTES < 0 {
					buffer = resizeBuffer(buffer, lastBufferIndex)
				}

				packet := make([]byte, config.SERVER_PACKET_SIZE_BYTES)
				_, err := client.Read(packet)

				fmt.Printf("string(packet): %v\n", string(packet))

				if err != nil {
					if len(buffer) > 0 {
						lastBufferIndex = 0
					}
					continue
				}

				startRuneIndex := strings.IndexByte(string(packet), '\\')

				// fmt.Printf("startRuneIndex: %v\n", startRuneIndex)
				// fmt.Printf("lastBufferIndex: %v\n", lastBufferIndex)

				// error case
				if lastBufferIndex == 0 && startRuneIndex == -1 {
					fmt.Println("error case")
					continue
				}

				// finished reading previous packet
				if lastBufferIndex > 0 && startRuneIndex > 0 {
					fmt.Println("finished reading previous packet")
					// startRuneIndex = endRuneIndex
					endRuneIndex := startRuneIndex
					copy(buffer[(lastBufferIndex+1):], packet[:endRuneIndex])

					dispatchToHandler(buffer, handler)

					lastBufferIndex = 0
					continue
				}

				// reading packet with no finish
				if lastBufferIndex > 0 && startRuneIndex < 0 {
					fmt.Println("reading packet with no finish")

					lastBufferIndex += copy(buffer[(lastBufferIndex+1):], packet)

					continue
				}

				// started new packet
				if lastBufferIndex == 0 && startRuneIndex >= 0 {
					fmt.Println("started new packet")
					endRuneIndex := strings.IndexByte(string(packet[(startRuneIndex+1):]), '\\')

					if endRuneIndex == -1 {
						lastBufferIndex += copy(buffer, packet[(startRuneIndex+1):])
						continue
					}

					// fmt.Printf("packet[startRuneIndex:endRuneIndex]: %v\n", packet[startRuneIndex:endRuneIndex])

					// else
					dispatchToHandler(packet[startRuneIndex:endRuneIndex], handler)
					lastBufferIndex = 0

					continue
				}

			}
		}

	}
}
