package network

import (
	"fmt"
	"reflect"
)

type handlerCallback func(any)

type handlerData struct {
	Topic    string
	Callback handlerCallback
	Type     any
}

func newHandler(topic string, handler handlerCallback, t any) handlerData {
	return handlerData{
		Topic:    topic,
		Callback: handler,
		Type:     t,
	}
}

func RegisterNewHandler(topic string, callback handlerCallback, dataType any) {
	t := reflect.TypeOf(dataType)

	if t.Kind() != reflect.Pointer && t.Elem().Kind() != reflect.Struct {
		panic(fmt.Sprintf("dataType must be a struct, got %s", t.Kind()))
	}

	handler := newHandler(topic, callback, dataType)
	handlers = append(handlers, handler)
}
