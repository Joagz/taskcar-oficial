#include <stdio.h>
#include <string.h>

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c_imu.h"

#define I2C_SLAVE_ADDR            0x68
#define TIMEOUT_MS                100
#define DELAY_MS                  500
#define REGISTER_TABLE_SIZE_BYTES 6
#define SCALING_FACTOR            0.000061

#define MPU_ACCEL_BEGIN           0x3B
#define MPU_ACCEL_END             0x40
#define MPU_PWR_MGMT_1            0x6B
#define MPU_ACCEL_CONFIG          0x1C

#define MPU_2G_CONFIG             0b00000000
#define MPU_4G_CONFIG             0b00001000
#define MPU_8G_CONFIG             0b00010000
#define MPU_16G_CONFIG            0b00011000

static const char *TAG = "i2c_imu";

static esp_err_t set_i2c(void)
{
    i2c_config_t config = {};
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = 21;
    config.scl_io_num = 22;
    config.sda_pullup_en = true;
    config.scl_pullup_en = true;
    config.master.clk_speed = 400000;
    config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}

void mpu_configure()
{
    ESP_ERROR_CHECK(set_i2c());

    uint8_t data[2];

    // Wake up the MPU6050 (default is sleep mode)
    data[0] = MPU_PWR_MGMT_1;
    data[1] = 0x00;
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, I2C_SLAVE_ADDR, data, 2, pdMS_TO_TICKS(TIMEOUT_MS)));

    // Set accelerometer range to ±2g (for example)
    data[0] = MPU_ACCEL_CONFIG;
    data[1] = MPU_2G_CONFIG;
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, I2C_SLAVE_ADDR, data, 2, pdMS_TO_TICKS(TIMEOUT_MS)));
}

void read_accel_data(float *ret)
{
    uint8_t rx_data[REGISTER_TABLE_SIZE_BYTES];
    uint8_t command = MPU_ACCEL_BEGIN;

    esp_err_t err = i2c_master_write_read_device(
        I2C_NUM_0, I2C_SLAVE_ADDR, &command, sizeof(command),
        rx_data, REGISTER_TABLE_SIZE_BYTES, pdMS_TO_TICKS(TIMEOUT_MS));

    if (err == ESP_OK)
    {
        int16_t xAcc = (rx_data[0] << 8) | rx_data[1];
        int16_t yAcc = (rx_data[2] << 8) | rx_data[3];
        int16_t zAcc = (rx_data[4] << 8) | rx_data[5];

        float scaleFactor = 2.0 / 32768.0; // for 2g config

        ret[0] = xAcc * scaleFactor * 9.8;
        ret[1] = yAcc * scaleFactor * 9.8;
        ret[2] = zAcc * scaleFactor * 9.8;
    }
    else
    {
        printf("Failed to read accelerometer data: %s\n", esp_err_to_name(err));
    }
}
