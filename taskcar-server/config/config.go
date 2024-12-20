package config

import "time"

const defaultTIME = 1
const SERVER_PACKET_SIZE_BYTES = 5
const SERVER_BUFFER_SIZE_BYTES = SERVER_PACKET_SIZE_BYTES * 4
const SERVER_DEFAULT_START_TIME = time.Second * defaultTIME
