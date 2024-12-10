idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    "components/motor_driver"
                    "components/i2c_imu"
                    REQUIRES motor_driver imu_driver driver)
