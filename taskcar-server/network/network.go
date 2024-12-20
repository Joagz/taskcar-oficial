/*
API functions for server
*/
package network

import (
	"fmt"
	"os"
)

func Start(host, username, password string, port int) ServerData {
	srv := NewServerData(host, port, username, password)

	go InitTCP(&srv, srv.GetFlag())
	srv.Running = srv.Wait()

	if !srv.Running {
		fmt.Fprintf(os.Stderr, "Start: could not start server in %s:%d\n", host, port)
		os.Exit(-1)
	}

	return srv
}

func Connect(root_user, root_password, topic, hostname string, port int) *ClientData {
	cli := newClientData(root_user, root_password, topic)

	if cli.connectClientData(resolveAddr(hostname, port)) != nil {
		fmt.Fprintf(os.Stderr, "ConnectNewClient: could not connect to address %s:%d\n", hostname, port)
		return nil
	}

	if cli.Network == nil {
		fmt.Fprintf(os.Stderr, "ConnectNewClient: network is nil\n")
		return nil
	}

	return &cli
}
