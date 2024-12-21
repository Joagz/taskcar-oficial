package network

type handlerCallback func(any)

type handlerData struct {
	Topic    string
	Callback handlerCallback
	Type     any
}

func newHandler(topic string, handler handlerCallback) handlerData {
	return handlerData{
		Topic:    topic,
		Callback: handler,
	}
}

func RegisterNewHandler(topic string, callback handlerCallback) {
	handler := newHandler(topic, callback)
	handlers = append(handlers, handler)
}
