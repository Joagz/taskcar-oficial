package network

import (
	"errors"
	"fmt"
	"net"
	"os"
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
	listener      *net.TCPListener
}

func (srv ServerData) Address() string {
	return srv.Host + ":" + strconv.Itoa(srv.Port)
}

func NewServerData(host string, port int, root_user string, root_password string) ServerData {
	return ServerData{
		Host:          host,
		Port:          port,
		root_user:     root_user,
		root_password: encrypt(root_password),
		Running:       false,
	}
}

/*
Sends bytes to the client's registered handler
*/
func dispatchToHandler(buf []byte, handler *handlerData) {
	obj := Deserialize(buf, handler.Type)

	if obj == nil {
		handler.Callback(handler.Type)
		return
	}
}

/*
Determines a handler for the topic that the client registers.
*/
func handleConnection(buffer []byte) (*handlerData, *ClientData, error) {

	cli := ClientData{}
	err := Deserialize(buffer, &cli)

	if err != nil {
		fmt.Printf("err: %v\n", err)
		return nil, nil, err
	}

	if !checkCredentials(cli) {
		fmt.Println("invalid credentials")
		return nil, nil, errors.New("invalid credentials")
	}

	var handler *handlerData

	for _, item := range handlers {

		if item.Topic == cli.Topic {
			handler = &item
		}
	}

	if handler == nil {
		fmt.Println("no handler function registered")
		return nil, &cli, errors.New("no handler function registered")
	}

	return handler, &cli, nil
}

func readClient(client net.Conn) []byte {
	buffer := make([]byte, config.SERVER_BUFFER_SIZE_BYTES)
	lastBufferIndex := 0

	for {
		if cap(buffer)-config.SERVER_PACKET_SIZE_BYTES < 0 {
			buffer = resizeBuffer(buffer, lastBufferIndex)
		}

		packet := make([]byte, config.SERVER_PACKET_SIZE_BYTES)
		_, err := client.Read(packet)

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

			break
		}

		// reading packet with no finish
		if lastBufferIndex > 0 && startRuneIndex < 0 {
			fmt.Println("reading packet with no finish")
			lastBufferIndex += copy(buffer[(lastBufferIndex+1):], packet)

			continue
		}

		// started new packet
		if lastBufferIndex == 0 && startRuneIndex >= 0 {
			endRuneIndex := strings.IndexByte(string(packet[(startRuneIndex+1):]), '\\')

			if endRuneIndex == -1 {
				lastBufferIndex += copy(buffer, packet[(startRuneIndex+1):])
				continue
			}

			copy(buffer, packet[(startRuneIndex+1):(endRuneIndex+1)])

			break
		}

	}

	return buffer
}

/*
Reads incoming client data and dispatches it to the correspoding handler
*/
func readClientAndDispatch(client net.Conn, handler *handlerData) {
	buffer := make([]byte, config.SERVER_BUFFER_SIZE_BYTES)
	lastBufferIndex := 0

	for {
		if cap(buffer)-config.SERVER_PACKET_SIZE_BYTES < 0 {
			buffer = resizeBuffer(buffer, lastBufferIndex)
		}

		packet := make([]byte, config.SERVER_PACKET_SIZE_BYTES)
		_, err := client.Read(packet)

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
			continue
		}

		// finished reading previous packet
		if lastBufferIndex > 0 && startRuneIndex > 0 {
			// startRuneIndex = endRuneIndex
			endRuneIndex := startRuneIndex
			copy(buffer[(lastBufferIndex+1):], packet[:endRuneIndex])

			dispatchToHandler(buffer, handler)

			lastBufferIndex = 0
			continue
		}

		// reading packet with no finish
		if lastBufferIndex > 0 && startRuneIndex < 0 {
			lastBufferIndex += copy(buffer[(lastBufferIndex+1):], packet)

			continue
		}

		// started new packet
		if lastBufferIndex == 0 && startRuneIndex >= 0 {
			endRuneIndex := strings.IndexByte(string(packet[(startRuneIndex+1):]), '\\')

			if endRuneIndex == -1 {
				lastBufferIndex += copy(buffer, packet[(startRuneIndex+1):])
				continue
			}

			// fmt.Printf("packet[startRuneIndex:endRuneIndex]: %v\n", packet[startRuneIndex:endRuneIndex])

			// else
			dispatchToHandler(packet[startRuneIndex:(endRuneIndex+1)], handler)
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
			fmt.Println("client accept error")
			continue
		}

		buffer := readClient(client)
		handler, clientdata, err := handleConnection(buffer)

		if err != nil || clientdata == nil {
			fmt.Println("Closing client connection")
			client.Close()
			continue
		}

		if handler == nil {
			fmt.Fprintf(os.Stderr, "handler for packet with topic %v not registered\n", clientdata.Topic)
			continue
		}

		println("readClientAndDispatch")

		readClientAndDispatch(client, handler)

	}
}

// receives a pointer to a ServerData struct and a boolean channel
// that determines the running state of the server
func initTCP(srv *ServerData) {

	addr, err := net.ResolveTCPAddr("tcp", srv.Address())

	checkErrExit(err)

	listener, err := net.ListenTCP("tcp", addr)

	checkErrExit(err)

	srv.listener = listener

	srv.Running = true

	// returns only if an error occurs
	go acceptClients(listener)
}
