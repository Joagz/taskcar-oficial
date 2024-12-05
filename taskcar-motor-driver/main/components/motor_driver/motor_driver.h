#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1
} motor_n;

typedef struct
{
    motor_n motor;
    gpio_num_t in1_pin;
    gpio_num_t in2_pin;
    gpio_num_t en_pin;
    ledc_channel_t motor_channel;
    uint32_t current_duty;
    
    bool is_clockwise;
    bool is_halted;
} motor_t;

/**
 * @brief Writes a duty cycle to a specific motor.
 *
 * @param motor_n Motor identifier (0 for left motor, 1 for right motor).
 * @param duty PWM duty cycle (0-100% converted to 0-255 for 8-bit resolution).
 * @param is_clockwise Direction of motor rotation (true = clockwise, false = counter-clockwise).
 */
void motor_write(motor_n motor_n, uint32_t duty, bool is_clockwise);

/**
 * @brief Configures the motor driver hardware.
 *
 * @return `ESP_OK` on success, error code otherwise.
 */
esp_err_t configure_motor_driver();

#endif