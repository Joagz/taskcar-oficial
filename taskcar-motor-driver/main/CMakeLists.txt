idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    "components/motor_driver"
                    REQUIRES motor_driver driver)
