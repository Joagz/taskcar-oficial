package network

import (
	"fmt"
	"os"
)

var handlers []handlerData

func checkErrExit(e error) {
	if e != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %s\n", e.Error())
		os.Exit(-1)
	}
}
