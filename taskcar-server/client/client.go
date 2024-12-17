package client

import (
	"errors"
	"fmt"
	"net"
	"strings"
	"taskcar/data"
	"time"
)

type ClientData struct {
	root_user     string
	root_password string
	Topic         string
}

func NewClientData(root_user, root_password, topic string) ClientData {
	return ClientData{
		root_user:     root_user,
		root_password: root_password,
		Topic:         topic,
	}
}

func (c ClientData) Serialize() ([]byte, error) {

	// could check some attrbs

	return []byte(c.root_user + ";" + c.root_password + ";" + c.Topic + "\n"), nil
}

func (ClientData) Deserialize(data []byte) (data.Serializable, error) {
	values := strings.Split(string(data), ";")

	if len(values) < 3 {
		fmt.Print("invalid data length")
		return nil, errors.New("invalid data length")
	}

	root_user, root_password, topic := values[0], values[1], values[2]

	clientData := NewClientData(root_user, root_password, topic)

	return clientData, nil
}

func Write(conn *net.TCPConn, data []byte) error {
	bytes, err := conn.Write(data)

	if bytes == 0 || err != nil {
		fmt.Printf("err: %v\n", err)
		return errors.New("could not write to server")
	}

	time.Sleep(50 * time.Nanosecond)
	return nil
}

func (c ClientData) Connect(address string) (*net.TCPConn, error) {

	addr, err := net.ResolveTCPAddr("tcp", address)

	if err != nil {
		return nil, errors.New("could not resolve tcp address")
	}

	conn, err := net.DialTCP("tcp", nil, addr)

	if err != nil {
		return nil, errors.New("could not dial tcp")
	}

	bytes, _ := c.Serialize()

	b, err := conn.Write(bytes)

	if b == 0 || err != nil {
		fmt.Println("Client not connected")
		return nil, errors.New("could not connect client")
	}

	fmt.Println("Client connected")

	return conn, nil
}
