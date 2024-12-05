#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "soc/gpio_num.h"
#include <stdbool.h>
#include <stdint.h>

#define MOTOR_LEFT_CHANNEL    LEDC_CHANNEL_0
#define MOTOR_RIGHT_CHANNEL   LEDC_CHANNEL_1

#define MOTOR_LEFT_GPIO       CONFIG_MOTOR_LEFT_GPIO
#define MOTOR_RIGHT_GPIO      CONFIG_MOTOR_RIGHT_GPIO

#define MOTOR_LEFT_IN1        CONFIG_MOTOR_LEFT_IN1
#define MOTOR_LEFT_IN2        CONFIG_MOTOR_LEFT_IN2

#define MOTOR_RIGHT_IN1       CONFIG_MOTOR_RIGHT_IN1
#define MOTOR_RIGHT_IN2       CONFIG_MOTOR_RIGHT_IN2

#define MOTOR_DRIVER_DUTY_RES CONFIG_MOTOR_DRIVER_DUTY_RES
#define MOTOR_DRIVER_MAX_DUTY (1 << MOTOR_DRIVER_DUTY_RES)
#define MOTOR_DRIVER_PWM_FREQ CONFIG_MOTOR_DRIVER_PWM_FREQ

typedef enum
{
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1
} motor_side;

typedef enum
{
    MOTOR_WRITE_ERR = -1,
    MOTOR_OK = 0,
} motor_err;

typedef struct
{
    motor_side motor_n;
    gpio_num_t in1_pin;
    gpio_num_t in2_pin;
    gpio_num_t en_pin;
    ledc_channel_t pwm_channel;
    uint32_t current_duty;
    bool is_clockwise;
    bool is_halted;
} motor_t;

/**
 * @brief Makes the motor run specifying the corresponding configuration
 * @return MOTOR_OK on success, an error code otherwise
 * @param motor Motor structure, the attributres used by this function are
 *  current_duty, is_clockwise, in1_pin, in2_pin, en_pin and pwm_channel
 */
motor_err motor_write(motor_t *motor);

/**
 * @brief Halts the motor completely
 * @return MOTOR_OK on success, an error code otherwise
 * @param motor Motor structure, the attributres used by this function are
 *  is_clockwise, in1_pin, in2_pin, en_pin and pwm_channel
 */
motor_err motor_stop(motor_t *motor);

/**
 * @brief Configures the motor driver hardware.
 *
 * @return `ESP_OK` on success, error code otherwise.
 */
esp_err_t motor_driver_init();

#endif