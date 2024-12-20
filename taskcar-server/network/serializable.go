package network

import (
	"fmt"
	"os"
	"reflect"
	"strconv"
	"strings"
)

type Serializable interface {
	Serialize() ([]byte, error)
	Deserialize([]byte) (Serializable, error)
}

func boolToString(v bool) string {
	if v {
		return "true"
	}

	return "false"
}

func stringToBool(v string) bool {
	return v == "true"
}

func DefaultSerializer(values ...any) string {

}

func DefaultDeserializer(data []byte, t any) error {
	values := strings.Split(string(data), ";")

	val := reflect.ValueOf(t)
	st := val.Elem()

	if val.Kind() != reflect.Ptr || val.Elem().Kind() != reflect.Struct {
		fmt.Println("Expected a pointer to a struct")
		return fmt.Errorf("expected a pointer to a struct")
	}

	if len(values) != st.NumField() {
		return nil
	}

	for i := 0; i < st.NumField(); i++ {
		v := st.Field(i)

		if !v.CanSet() {
			continue
		}

		kind := v.Kind()

		if kind == reflect.String {
			v.SetString(values[i])
			continue
		}

		if kind == reflect.Bool {
			v.SetBool(stringToBool(values[i]))
			continue
		}

		if kind >= reflect.Int && kind <= reflect.Int64 {
			conv, err := strconv.ParseInt(values[i], 10, 64)

			if err != nil {
				fmt.Fprintf(os.Stderr, "DefaultDeserializer: Error converting string '%s' to int: %v\n", values[i], err)
				return nil
			}

			v.SetInt(conv)
			continue
		}

		if kind == reflect.Float32 || kind == reflect.Float64 {
			conv, err := strconv.ParseFloat(values[i], 64)

			if err != nil {
				fmt.Fprintf(os.Stderr, "DefaultDeserializer: Error converting string '%s' to float: %v\n", values[i], err)
				return nil
			}

			v.SetFloat(conv)
			continue
		}

		if kind >= reflect.Uint || kind <= reflect.Uint64 {
			conv, err := strconv.ParseUint(values[i], 10, 64)

			if err != nil {
				fmt.Fprintf(os.Stderr, "DefaultDeserializer: Error converting string '%s' to float: %v\n", values[i], err)
				return nil
			}
			v.SetUint(conv)
			continue
		}

	}
	return nil
}
