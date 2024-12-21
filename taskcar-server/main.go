package main

import (
	"fmt"
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

func callback(obj any) {
	messages.Push(&obj)
}

func main() {
	// network.RegisterNewHandler("example", callback, DataType{})
	// network.Start("localhost", "", "", 7000)

	data := &DataType{
		Value1: "Hello",
		Value2: ",",
		Value3: "world",
	}

	// sendData()

	bytes, _ := network.Serialize(data)

	vals := &DataType{}
	network.Deserialize(bytes, vals)

	fmt.Printf("vals: %v\n", vals)

}
