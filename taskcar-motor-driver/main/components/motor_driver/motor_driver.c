#include "driver/ledc.h"
#include "esp_log.h"
#include <stdio.h>

#include "motor_driver.h"

#define CONFIG_LOG_MAXIMUM_LEVEL ESP_LOG_VERBOSE

#define MOTOR_LEFT_CHANNEL LEDC_CHANNEL_0
#define MOTOR_RIGHT_CHANNEL LEDC_CHANNEL_1

#define MOTOR_LEFT_GPIO GPIO_NUM_5
#define MOTOR_RIGHT_GPIO 14 

#define MOTOR_LEFT_IN1 GPIO_NUM_25
#define MOTOR_LEFT_IN2 GPIO_NUM_26

#define MOTOR_RIGHT_IN1 23
#define MOTOR_RIGHT_IN2 32

void configure_gpio()
{
    gpio_set_direction(MOTOR_LEFT_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_RIGHT_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_LEFT_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_LEFT_IN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_RIGHT_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_RIGHT_IN2, GPIO_MODE_OUTPUT);
}

esp_err_t configure_ledc_channels()
{
    ledc_channel_config_t config_left = {
        .gpio_num = MOTOR_LEFT_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_FADE_END,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };

    ledc_channel_config_t config_right = {
        .gpio_num = MOTOR_RIGHT_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_FADE_END,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };

    esp_err_t err = ledc_channel_config(&config_left);
    if (err != ESP_OK)
    {
        ESP_LOGE("driver", "error setting left pwm channel");
        return err;
    }

    err = ledc_channel_config(&config_right);
    if (err != ESP_OK)
    {
        ESP_LOGE("driver", "error setting right pwm channel");
        return err;
    }

    return ESP_OK;
}

esp_err_t configure_ledc_timer()
{
    ledc_timer_config_t config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 10*1000, // 10KHz por ejemplo, TODO: cambiar
        .clk_cfg = LEDC_AUTO_CLK
    };

    esp_err_t err = ledc_timer_config(&config);

    if (err != ESP_OK)
    {
        ESP_LOGE("driver", "error setting configuration for timer");
        return err;
    }

    return ESP_OK;
}

esp_err_t configure_motor_driver()
{
    configure_gpio();

    esp_err_t err;

    if ((err = configure_ledc_channels()) != ESP_OK)
        return err;
    if ((err = configure_ledc_timer()) != ESP_OK)
        return err;

    return ESP_OK;
}

void writem(uint32_t duty, bool is_clockwise, int motor_in1, int motor_in2, int motor_channel)
{
    if (is_clockwise)
    {
        gpio_set_level(motor_in1, 1);
        gpio_set_level(motor_in2, 0);
    }
    else
    {
        gpio_set_level(motor_in1, 0);
        gpio_set_level(motor_in2, 1);
    }

    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor_channel);
}

void motor_write(uint8_t motor_n, uint32_t duty, bool is_clockwise)
{

    switch (motor_n)
    {
    case 0:
        writem(duty, is_clockwise, MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, MOTOR_LEFT_CHANNEL);
        break;
    case 1:
        writem(duty, is_clockwise, MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, MOTOR_RIGHT_CHANNEL);
        break;
    default:
        ESP_LOGE("driver", "motor_n attribute incorrect");
    }
}
