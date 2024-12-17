package server

import (
	"errors"
	"fmt"
	"net"
	"os"
	"strings"
	"taskcar/client"
	"taskcar/data"
)

var handlers []data.HandlerData

func checkErrExit(e error) {
	if e != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %s\n", e.Error())
		os.Exit(-1)
	}
}

func checkCredentials(cli client.ClientData) bool {
	return true // TODO
}

func RegisterNewHandler(handler data.HandlerData) {
	// ...
	handlers = append(handlers, handler)
}

/*
Determines a handler for the topic that the client registers.
*/
func handleConnection(conn net.Conn) (*data.HandlerData, error) {
	buffer := make([]byte, 1024)
	bytes, err := conn.Read(buffer)

	buffer = []byte(strings.Split(string(buffer), "\n")[0])

	if bytes == 0 {
		return nil, errors.New("no data received")
	}

	if err != nil {
		fmt.Printf("err: %v\n", err)
		return nil, err
	}

	// weird
	var cli client.ClientData
	obj, _ := cli.Deserialize(buffer)
	cli = obj.(client.ClientData)

	if !checkCredentials(cli) {
		return nil, errors.New("invalid credentials")
	}

	var handler *data.HandlerData

	for _, item := range handlers {

		if item.Topic == cli.Topic {
			handler = &item
		}
	}

	if handler == nil {
		return nil, errors.New("no handler function registered")
	}

	return handler, nil
}
