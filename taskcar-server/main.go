package main

import (
	"errors"
	"fmt"
	"strings"
	"taskcar/network"
	"taskcar/stack"
	"time"
)

var messages stack.Stack

type DataType struct {
	value1 string
	value2 string
	value3 string
}

func (d DataType) Serialize() ([]byte, error) {
	return []byte("\\" + d.value1 + ";" + d.value2 + ";" + d.value3 + "\\"), nil
}

func (DataType) Deserialize(data []byte) (network.Serializable, error) {
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

func callback(obj network.Serializable) {
	data := obj.(any)

	messages.Push(&data)
}

func main() {
	messages = stack.New(15)

	network.RegisterNewHandler("example", callback, DataType{})

	network.Start("localhost", "", "", 7000)
	cli := network.Connect("", "", "example", "localhost", 7000)

	data := DataType{
		value1: "Hello",
		value2: ",",
		value3: "world",
	}

	cli.Write(data)
	cli.Write(data)
	cli.Write(data)
	cli.Write(data)
	cli.Write(data)
	cli.Write(data)

	time.Sleep(time.Second)
	val := messages.Pop()

	for val != nil {
		fmt.Printf("val: %v\n", *val)
		val = messages.Pop()
	}

}
