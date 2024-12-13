/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "math.h"
#include "sdkconfig.h"

#include "motor_driver.h"
#include "mpu6050.h"
#include <stdio.h>

void app_main(void)
{
	esp_err_t err = motor_driver_init();

	if (err != ESP_OK)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		esp_restart();
	}

	mpu6050_t mpu6050 = mpu6050_new(
		MPU6050_ACCEL_2G,
		MPU6050_GYRO_250,
		MPU6050_DLPF_0,
		MPU6050_SLAVE_ADDR_1);

	mpu6050_configure(&mpu6050);
	// mpu6050_calibrate(&mpu6050);
	// mpu6050_stddev(&mpu6050, 100);

	while (1)
	{
		mpu6050_accelgyro_data(&mpu6050);

		ESP_LOGI("m/s^2", "X: %.1f \t %d, Y: %.1f \t %d, Z: %.1f \t %d",
				 (mpu6050.x_acc), mpu6050.x_acc_frw,
				 (mpu6050.y_acc), mpu6050.y_acc_frw,
				 (mpu6050.z_acc), mpu6050.z_acc_frw);

		ESP_LOGI("deg/sec", "X: %.1f \t %d, Y: %.1f \t %d, Z: %.1f \t %d",
				 (mpu6050.x_deg), mpu6050.x_deg_clkw,
				 (mpu6050.y_deg), mpu6050.y_deg_clkw,
				 (mpu6050.z_deg), mpu6050.z_deg_clkw);

		vTaskDelay(pdTICKS_TO_MS(1));
	}
}
