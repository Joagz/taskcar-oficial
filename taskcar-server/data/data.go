package data

type HandlerCallback func(Serializable)

type Serializable interface {
	Serialize() ([]byte, error)
	Deserialize([]byte) (Serializable, error)
}

type HandlerData struct {
	Topic    string
	Callback HandlerCallback
	Serializable
}

func Encrypt(s string) string {
	return s
}

func NewHandler(topic string, handler HandlerCallback, serializable Serializable) HandlerData {
	return HandlerData{
		Topic:        topic,
		Callback:     handler,
		Serializable: serializable,
	}
}
