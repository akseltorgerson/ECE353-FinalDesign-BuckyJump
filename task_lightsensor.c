/*
 * task_lightsensor.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_LightSensor_Handle;

void Task_LightSensor(void *pvParameters) {

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}



