#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * @brief Writes a duty cycle to a specific motor.
 *
 * @param motor_n Motor identifier (0 for left motor, 1 for right motor).
 * @param duty PWM duty cycle (0-100% converted to 0-255 for 8-bit resolution).
 * @param is_clockwise Direction of motor rotation (true = clockwise, false = counter-clockwise).
 */
void motor_write(uint8_t motor_n, uint32_t duty, bool is_clockwise);

/**
 * @brief Configures the motor driver hardware.
 *
 * @return `ESP_OK` on success, error code otherwise.
 */
esp_err_t configure_motor_driver();

#endif