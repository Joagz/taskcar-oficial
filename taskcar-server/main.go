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
	cli := network.Connect("", "", "example", "localhost", 7000)
	messages = stack.New(15)

	data := DataType{
		Value1: "Hello",
		Value2: ",",
		Value3: "world",
	}

	bytes, _ := network.Serialize(&data)

	cli.Write(bytes)

	time.Sleep(time.Second)

	val := messages.Pop()

	for val != nil {
		fmt.Printf("val: %v\n", *val)
		val = messages.Pop()
	}

}

func callback(obj any) {
	fmt.Printf("obj: %v\n", obj)
	messages.Push(&obj)
}

func main() {
	network.RegisterNewHandler("example", callback, &DataType{})
	network.Start("localhost", "", "", 7000)

	sendData()
}
