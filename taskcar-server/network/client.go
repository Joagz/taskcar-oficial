package network

import (
	"errors"
	"fmt"
	"net"
	"os"
	"strings"
	"taskcar/config"
)

type ClientData struct {
	root_user     string
	root_password string
	Topic         string
	Connected     bool
	Network       *net.TCPConn
}

func newClientData(root_user, root_password, topic string) ClientData {
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

func (ClientData) Deserialize(data []byte) (Serializable, error) {
	values := strings.Split(string(data), ";")

	if len(values) < 3 {
		fmt.Print("invalid data length")
		return nil, errors.New("invalid data length")
	}

	root_user, root_password, topic := values[0], values[1], values[2]

	clientData := newClientData(root_user, root_password, topic)

	return clientData, nil
}

func (cli ClientData) Write(toWrite Serializable) {
	bytes, _ := toWrite.Serialize()

	totalToWrite := len(bytes)
	totalWritten := 0

	for totalWritten < totalToWrite {
		written, err := cli.Network.Write(bytes[totalWritten:(totalWritten + config.SERVER_PACKET_SIZE_BYTES)])
		totalWritten += written

		if err != nil {
			fmt.Fprintf(os.Stderr, "Write: error sending packet, discarding")
			return
		}
	}
}

func (c *ClientData) connectClientData(address string) error {

	addr, err := net.ResolveTCPAddr("tcp", address)

	if err != nil {
		return errors.New("could not resolve tcp address")
	}

	conn, err := net.DialTCP("tcp", nil, addr)

	if err != nil {
		return errors.New("could not dial tcp")
	}

	bytes, _ := c.Serialize()

	b, err := conn.Write(bytes)

	if b == 0 || err != nil {
		fmt.Fprintf(os.Stderr, "connectClientData: could not connect client")
		return errors.New("could not connect client")
	}

	c.Connected = true
	c.Network = conn

	return nil
}
