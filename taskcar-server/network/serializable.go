package network

import (
	"fmt"
	"os"
	"reflect"
	"strconv"
	"strings"
	"taskcar/config"
)

func boolToString(v bool) string {
	if v {
		return "true"
	}

	return "false"
}

func stringToBool(v string) bool {
	return v == "true"
}

func Serialize(t any) ([]byte, error) {
	types := reflect.TypeOf(t)
	vals := reflect.ValueOf(t)
	stType := types.Elem()
	stVal := vals.Elem()

	if vals.Kind() != reflect.Ptr || vals.Elem().Kind() != reflect.Struct {
		fmt.Println("Expected a pointer to a struct")
		return nil, fmt.Errorf("expected a pointer to a struct")
	}

	var finalString string

	for i := 0; i < stType.NumField(); i++ {
		typeField := stType.Field(i)
		valueField := stVal.Field(i)

		var key, value string
		keystr := typeField.Tag.Get(config.DEFAULT_STRUCT_TAG)

		if keystr != "" {
			key = keystr
		} else {
			key = typeField.Name
		}

		value = valueField.String()

		finalString += fmt.Sprintf("%s:%s\n", key, value)
	}

	return []byte(finalString[:(len(finalString) - 1)]), nil

}

// Receives a byte array and a pointer to a struct. Deserializes the byte array
// and modifies the attributes in the struct
func Deserialize(data []byte, t any) error {

	val := reflect.ValueOf(t)
	st := val.Elem()

	if val.Kind() != reflect.Ptr || val.Elem().Kind() != reflect.Struct {
		fmt.Println("Expected a pointer to a struct")
		return fmt.Errorf("expected a pointer to a struct")
	}

	keyvalues := strings.Split(string(data), "\n")

	values := make([]string, len(keyvalues))
	for i, r := range keyvalues {
		a := strings.Split(r, ":")

		if len(a) > 1 {
			values[i] = a[1]
		}
	}

	fmt.Printf("values: %v\n", values)

	if len(values) != st.NumField() {
		return nil
	}
	// todo: deserialize unordered struct string
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
