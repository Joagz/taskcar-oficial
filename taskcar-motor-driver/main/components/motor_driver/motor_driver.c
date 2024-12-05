#include "motor_driver.h"

void configure_gpio()
{
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_LEFT_GPIO, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_RIGHT_GPIO, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_LEFT_IN1, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_LEFT_IN2, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_RIGHT_IN1, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_RIGHT_IN2, GPIO_MODE_OUTPUT));
}

esp_err_t motor_configure(motor_t *motor)
{

    if (motor == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    motor->is_halted = true;
    motor->current_duty = 0;

    ledc_channel_config_t config = {
        .gpio_num = motor->en_pin,
        .channel = motor->pwm_channel,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
    };

    esp_err_t err = ledc_channel_config(&config);
    if (err != ESP_OK)
    {
        ESP_LOGE("driver", "error setting left pwm channel");
        return err;
    }

    return ESP_OK;
}

esp_err_t configure_ledc_timer()
{
    ledc_timer_config_t config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = MOTOR_DRIVER_DUTY_RES,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = MOTOR_DRIVER_PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK};

    esp_err_t err = ledc_timer_config(&config);

    if (err != ESP_OK)
    {
        ESP_LOGE("driver", "error setting configuration for timer");
        return err;
    }

    return ESP_OK;
}

esp_err_t motor_driver_init()
{
    configure_gpio();

    ESP_ERROR_CHECK(configure_ledc_timer());
    return ESP_OK;
}

esp_err_t writem(uint32_t duty, bool is_clockwise, int motor_in1, int motor_in2, int motor_channel)
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

    esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channel, duty);

    if (err != ESP_OK)
    {
        ESP_LOGE("writem", "ledc_set_duty");
        return err;
    }

    err = ledc_update_duty(LEDC_LOW_SPEED_MODE, motor_channel);

    if (err != ESP_OK)
    {
        ESP_LOGE("writem", "ledc_update_duty");
        return err;
    }

    return ESP_OK;
}

motor_err motor_stop(motor_t *motor)
{
    if (motor == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if (writem(0, motor->is_clockwise, motor->in1_pin, motor->in2_pin, motor->pwm_channel) != ESP_OK)
    {
        ESP_LOGE("motor_write", "error writing");
        return MOTOR_WRITE_ERR;
    }

    motor->is_halted = false;

    return MOTOR_OK;
}

motor_err motor_write(motor_t *motor)
{

    if (motor == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if (motor->current_duty == 0)
    {
        return motor_stop(motor);
    }

    if (writem(motor->current_duty, motor->is_clockwise, motor->in1_pin, motor->in2_pin, motor->pwm_channel) != ESP_OK)
    {
        ESP_LOGE("motor_write", "error writing");
        return MOTOR_WRITE_ERR;
    }

    motor->is_halted = false;

    return MOTOR_OK;
}
