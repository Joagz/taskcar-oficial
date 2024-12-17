package server

import (
	"errors"
	"strconv"
	"strings"
)

type ServerData struct {
	Host          string
	Port          int
	root_user     string
	root_password string // ? encrypt
	Running       bool
}

type ClientData struct {
	root_user     string
	root_password string
	Topic         string
}

func (clientData *ClientData) Deserialize(data []byte) error {
	values := strings.Split(string(data), ";")

	if len(values) < 3 {
		return errors.New("invalid data length")
	}

	root_user, root_password, topic := values[0], values[1], values[2]

	clientData.root_user = root_user
	clientData.root_password = root_password
	clientData.Topic = topic
	return nil
}

func encrypt(s string) string {
	return s
}

func NewServerData(host string, port int, root_user string, root_password string) ServerData {
	return ServerData{
		Host:          host,
		Port:          port,
		root_user:     root_user,
		root_password: encrypt(root_password),
	}
}

func (srv ServerData) address() string {
	return srv.Host + ":" + strconv.Itoa(srv.Port)
}
