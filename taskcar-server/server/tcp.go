package server

import (
	"errors"
	"fmt"
	"net"
	"os"
	"taskcar/data"
)

var handlers []data.HandlerData

func checkErrExit(e error) {
	if e != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %s\n", e.Error())
		os.Exit(-1)
	}
}

func checkCredentials(client ClientData) bool {
	return true // TODO
}

/*
Determines a handler for the topic that the client registers.
*/
func handleConnection(conn *net.TCPConn) (*data.HandlerData, error) {
	var buffer []byte

	bytes, err := conn.Read(buffer)

	if bytes == 0 || err != nil {
		return nil, errors.New("error reading data")
	}

	var client ClientData = ClientData{}
	client.Deserialize(buffer)

	if !checkCredentials(client) {
		return nil, errors.New("invalid credentials")
	}

	var handler *data.HandlerData

	for _, item := range handlers {
		if item.Topic == client.Topic {
			handler = &item
		}
	}

	if handler.Callback == nil {
		return nil, errors.New("no handler function registered")
	}

	return handler, nil
}

// should this block?
/*
This server receives packets from its clients and does not send any response
*/
func (srv ServerData) TCPReceiverServer() {
	addr, err := net.ResolveTCPAddr("tcp", srv.address())

	checkErrExit(err)

	listener, err := net.ListenTCP("tcp", addr)

	checkErrExit(err)

	for {
		client, err := listener.AcceptTCP()
		go func() {
			if err == nil {
				handler, err := handleConnection(client)

				if err != nil {
					client.Close()
					fmt.Println(err.Error())
					return
				}

				for {
					var buffer []byte

					_, err := client.Read(buffer)

					if err != nil {
						continue
					}

					obj, _ := handler.Deserialize(buffer)

					handler.Callback(obj)
				}
			}
		}()
	}
}
