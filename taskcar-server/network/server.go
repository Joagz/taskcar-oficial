package network

import (
	"errors"
	"fmt"
	"net"
	"strconv"
	"strings"
	"taskcar/config"
)

type ServerData struct {
	Host          string
	Port          int
	root_user     string
	root_password string // ? encrypt
	Running       bool
	flag          chan bool
}

func (s ServerData) GetFlag() chan bool {
	return s.flag
}

func (s ServerData) Wait() bool {
	return <-s.flag
}

func (srv ServerData) Address() string {
	return srv.Host + ":" + strconv.Itoa(srv.Port)
}

func NewServerData(host string, port int, root_user string, root_password string) ServerData {
	flag := make(chan bool)

	return ServerData{
		Host:          host,
		Port:          port,
		root_user:     root_user,
		root_password: encrypt(root_password),
		flag:          flag,
	}
}

/*
Sends bytes to the client's registered handler
*/
func dispatchToHandler(buf []byte, handler *handlerData) {
	obj, _ := handler.Deserialize([]byte(buf))
	fmt.Println("handler dispatched")

	if obj != nil {
		handler.Callback(obj)
		return
	}

	fmt.Println("object couldn't be deserialized")
}

/*
Determines a handler for the topic that the client registers.
*/
func handleConnection(conn net.Conn) (*handlerData, *ClientData, error) {
	buffer := make([]byte, 1024)
	bytes, err := conn.Read(buffer)

	buffer = []byte(strings.Split(string(buffer), "\n")[0])

	if bytes == 0 {
		return nil, nil, errors.New("no data received")
	}

	if err != nil {
		fmt.Printf("err: %v\n", err)
		return nil, nil, err
	}

	// deserialize client data
	var cli ClientData
	obj, _ := cli.Deserialize(buffer)
	cli = obj.(ClientData)

	if !checkCredentials(cli) {
		return nil, nil, errors.New("invalid credentials")
	}

	var handler *handlerData

	for _, item := range handlers {

		if item.Topic == cli.Topic {
			handler = &item
		}
	}

	if handler == nil {
		return nil, &cli, errors.New("no handler function registered")
	}

	return handler, &cli, nil
}

/*
Reads incoming client data
*/
func readClient(client net.Conn, handler *handlerData) {
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

/*
Accepts a client and tries to determine the associated handler
*/
func acceptClients(listener net.Listener) {
	for {
		client, err := listener.Accept()

		if err != nil {
			break
		}

		handler, clientdata, err := handleConnection(client)

		if err != nil {
			client.Close()
			fmt.Println(err.Error())
			continue
		}

		if handler == nil {
			fmt.Printf("handler for packet with topic %v not registered\n", clientdata.Topic)
			continue
		}

		go readClient(client, handler)

	}
}

// receives a pointer to a ServerData struct and a boolean channel
// that determines the running state of the server
func InitTCP(srv *ServerData, running chan bool) {

	addr, err := net.ResolveTCPAddr("tcp", srv.Address())

	checkErrExit(err)

	listener, err := net.ListenTCP("tcp", addr)

	checkErrExit(err)

	running <- true

	// returns only if an error occurs
	acceptClients(listener)

	running <- false
}
