package network

type Serializable interface {
	Serialize() ([]byte, error)
	Deserialize([]byte) (Serializable, error)
}
