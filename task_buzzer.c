/*
 * task_buzzer.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Buzzer_Handle;

void Task_Buzzer(void *pvParameters) {

    uint32_t ulEventToProcess;

    while (1) {

        // wake on a task notification
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //music_play_sound();


        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


