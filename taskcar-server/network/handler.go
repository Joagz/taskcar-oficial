package network

type handlerCallback func(Serializable)

type handlerData struct {
	Topic    string
	Callback handlerCallback
	Serializable
}

func newHandler(topic string, handler handlerCallback, serializable Serializable) handlerData {
	return handlerData{
		Topic:        topic,
		Callback:     handler,
		Serializable: serializable,
	}
}

func RegisterNewHandler(topic string, callback handlerCallback, serializable Serializable) {
	handler := newHandler(topic, callback, serializable)
	handlers = append(handlers, handler)
}
