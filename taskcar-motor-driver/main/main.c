/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_driver.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <stdio.h>

void app_main(void)
{
    esp_err_t err = motor_driver_init();

    if (err != ESP_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    }

}
