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
	Value1 string `serv:"value_1"`
	Value2 string `serv:"value_2"`
	Value3 string `serv:"value_3"`
}

func (d DataType) Serialize() ([]byte, error) {
	return []byte("\\" + d.Value1 + ";" + d.Value2 + ";" + d.Value3 + "\\"), nil
}

func (DataType) Deserialize(data []byte) (network.Serializable, error) {
	values := strings.Split(string(data), ";")

	if len(values) < 3 {
		return nil, errors.New("invalid attribute length")
	}

	value1, value2, value3 := values[0], values[1], values[2]

	return DataType{
		Value1: value1,
		Value2: value2,
		Value3: value3,
	}, nil

}

func sendData() {
	messages = stack.New(15)

	cli := network.Connect("", "", "example", "localhost", 7000)

	data := DataType{
		Value1: "Hello",
		Value2: ",",
		Value3: "world",
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

func callback(obj network.Serializable) {
	data := obj.(any)

	messages.Push(&data)
}

func main() {
	// network.RegisterNewHandler("example", callback, DataType{})
	// network.Start("localhost", "", "", 7000)
	data := DataType{
		Value1: "Hello",
		Value2: ",",
		Value3: "world",
	}
	// sendData()

	bytes, _ := data.Serialize()
	vals := &DataType{}
	network.DefaultDeserializer(bytes, vals)

	fmt.Printf("vals: %v\n", vals)

}
