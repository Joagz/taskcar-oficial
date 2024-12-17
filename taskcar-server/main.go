package main

import (
	"errors"
	"fmt"
	"os"
	"strings"
	"taskcar/client"
	"taskcar/data"
	"taskcar/server"
	"taskcar/stack"
	"time"
)

type DataType struct {
	value1 string
	value2 string
	value3 string
}

func (d DataType) Serialize() ([]byte, error) {
	return []byte(d.value1 + ";" + d.value2 + ";" + d.value3 + "\n"), nil
}

func (DataType) Deserialize(data []byte) (data.Serializable, error) {

	values := strings.Split(string(data), ";")

	if len(values) < 3 {
		return nil, errors.New("invalid attribute length")
	}

	value1, value2, value3 := values[0], values[1], values[2]

	return DataType{
		value1: value1,
		value2: value2,
		value3: value3,
	}, nil

}

var messages stack.Stack

func callback(obj data.Serializable) {
	data := obj.(any)

	messages.Push(&data)
}

func main() {
	messages = stack.New(15)
	handler := data.NewHandler("example", callback, DataType{})
	server.RegisterNewHandler(handler)
	srv := server.NewServerData("localhost", 5000, "", "")

	flag := make(chan server.ServerData)

	go server.InitTCP(srv, flag)
	srv = <-flag

	if !srv.Running {
		os.Exit(-1)
	}

	fmt.Println("Server initialization finished")

	fmt.Printf("(<-flag): %v\n", srv)

	cli := client.NewClientData("a", "b", "example")

	conn, err := cli.Connect(srv.Address())

	if err != nil {
		fmt.Print(err)
		return
	}

	data := DataType{
		value1: "Hello",
		value2: ",",
		value3: "world!",
	}

	bytes, _ := data.Serialize()
	buffer := make([]byte, 512)

	for bytes_written, i := 0, 0; bytes_written < 512; bytes_written++ {
		if i == len(bytes) {
			i = 0
		}

		if 512-bytes_written < len(bytes) {
			break
		}

		buffer[bytes_written] = bytes[i]
		i++
	}

	client.Write(conn, buffer)

	time.Sleep(1 * time.Second)

	val := messages.Pop()

	for val != nil {
		fmt.Printf("val: %v\n", *val)
		val = messages.Pop()
	}

	srv = <-flag

	if !srv.Running {
		fmt.Println("server closed")
	}
}
