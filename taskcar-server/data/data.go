package data

type HandlerCallback func(any)

type Serializable interface {
	Serialize(any) ([]byte, error)
	Deserialize([]byte) (any, error)
}

type HandlerData struct {
	Topic    string
	Callback HandlerCallback
	Serializable
}

func NewHandler(topic string, handler HandlerCallback) HandlerData {
	return HandlerData{
		Topic:    topic,
		Callback: handler,
	}
}
