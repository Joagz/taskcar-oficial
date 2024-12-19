package main

import (
	"errors"
	"fmt"
	"strings"
	"taskcar/client"
	"taskcar/data"
	"taskcar/server"
	"taskcar/stack"
	"time"
)

const SERVER_PACKET_SIZE_BYTES = 128

type DataType struct {
	value1 string
	value2 string
	value3 string
}

func (d DataType) Serialize() ([]byte, error) {
	return []byte("\\" + d.value1 + ";" + d.value2 + ";" + d.value3 + "\\"), nil
}

func (DataType) Deserialize(data []byte) (data.Serializable, error) {
	fmt.Printf("string(data): %v\n", string(data))
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

	fmt.Printf("data: %v\n", data)

	messages.Push(&data)
}

func main() {
	messages = stack.New(15)

	handler := data.NewHandler("example", callback, DataType{})
	server.RegisterNewHandler(handler)

	srv := server.Start("localhost", "", "", 5000)
	time.Sleep(1 * time.Second)

	data := DataType{
		value1: "Hello",
		value2: ",",
		value3: "world",
	}

	cli := client.NewClientData("", "", "example")

	if cli.Connect(srv.Address()) != nil {
		fmt.Println("client connection failed")
		return
	}

	if cli.Network == nil {
		fmt.Println("network is nil")
		return
	}

	cli.SendPacket(data)

	time.Sleep(1 * time.Second)

	val := messages.Pop()

	for val != nil {
		fmt.Printf("val: %v\n", *val)
		val = messages.Pop()
	}

	srv.Wait()
}
