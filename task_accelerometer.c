/*
 * task_accelerometer.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Accelerometer_Handle;

void Task_Accelerometer(void *pvParameters) {

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
