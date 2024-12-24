package network

import (
	"errors"
	"fmt"
	"net"
	"os"
	"taskcar/config"
)

type ClientData struct {
	Topic         string `serv:"topic"`
	Connected     bool   `serv:"connected"`
	Root_password string `serv:"root_password"`
	Root_user     string `serv:"root_user"`
	network       *net.TCPConn
}

func newClientData(root_user, root_password, topic string) ClientData {
	return ClientData{
		Root_user:     root_user,
		Root_password: root_password,
		Topic:         topic,
	}
}

func (cli ClientData) Write(bytes []byte) {

	totalToWrite := len(bytes)
	totalWritten := 0

	for totalWritten < totalToWrite {
		written, err := cli.network.Write(bytes[totalWritten:(totalWritten + min(config.SERVER_PACKET_SIZE_BYTES, len(bytes)))])
		totalWritten += written

		if err != nil {
			fmt.Printf("err.Error(): %v\n", err.Error())
			fmt.Fprintf(os.Stderr, "Write: error sending packet, discarding\n")
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
	c.network = conn

	return nil
}
