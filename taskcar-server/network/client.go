package network

import (
	"errors"
	"fmt"
	"net"
	"os"
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

func (cli ClientData) Write(value any) {

	bytes, err := Serialize(value)

	if err != nil {
		fmt.Fprintf(os.Stderr, "Write: invalid object passedn")
		return
	}

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

	bytes, _ := Serialize(c)

	b, err := conn.Write(bytes)

	if b == 0 || err != nil {
		fmt.Fprintf(os.Stderr, "connectClientData: could not connect client")
		return errors.New("could not connect client")
	}

	c.Connected = true
	c.Network = conn

	return nil
}
