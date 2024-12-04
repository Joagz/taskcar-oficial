/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_driver.h"

void app_main(void)
{
    esp_err_t err = configure_motor_driver();

    if(err!=ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    }

    motor_write(0, 255, true);
    motor_write(1, 255, true);
}
