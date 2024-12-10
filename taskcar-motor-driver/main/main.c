/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

	mpu6050_t mpu6050 = {
		.ADDRESS=MPU6050_SLAVE_ADDR_1,
		.AFS_SEL=MPU6050_ACCEL_2G,
		.GFS_SEL=MPU6050_GYRO_250,
		.DLPF_CFG=MPU6050_DLPF_6,
	};

	mpu6050_configure(&mpu6050);

	float accelValues[3];
	float gyroValues[3];
	while (true)
	{
		mpu6050_accelgyro_data(accelValues, gyroValues, MPU6050_ACCEL_2G, MPU6050_GYRO_250);

		ESP_LOGI("main", "X: %.3f m/s^2, Y: %.3f m/s^2, Z: %.3f m/s^2",
				 accelValues[0], accelValues[1], accelValues[2]);
		// ESP_LOGI("main", "X: %.3f deg, Y: %.3f deg, Z: %.3f deg",
		// 		 gyroValues[0], gyroValues[1], gyroValues[2]);

		vTaskDelay(pdTICKS_TO_MS(1));
	}
}
