package network

import (
	"strconv"
	"taskcar/config"
)

func resolveAddr(hostname string, port int) string {
	return hostname + ":" + strconv.Itoa(port)
}

func resizeBuffer(buffer []byte, lastBufferIndex int) []byte {
	return append(buffer[:lastBufferIndex], make([]byte, config.SERVER_PACKET_SIZE_BYTES)...)
}
