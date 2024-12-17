idf_component_register(
                    SRCS
                    "main.c"
                    INCLUDE_DIRS 
                    "."
                    "components/motor_driver"
                    "components/imu_driver"
                    "components/wifi_station"
                    REQUIRES
                    motor_driver
                    imu_driver
                    driver
                    wifi_station)
                    