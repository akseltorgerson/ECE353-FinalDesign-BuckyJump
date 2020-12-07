/*
 * task_platform.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Platform_Handle;

void platform_init(void) {

    // init platform queue

    // maybe init other things

}

void Task_Platform(void *pvParameters) {

    while(1) {


        // necessary task delay
        vTaskDelay(pdMS_TO_TICKS(10));

    }



}

