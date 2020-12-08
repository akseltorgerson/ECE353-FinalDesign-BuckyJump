/*
 * task_platform.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Platform_Handle;
QueueHandle_t Queue_Platform;

PLATFORM_t platforms[10];

void platform_init(void) {

    // init platform queue
    // not used right now, just here for testing
    Queue_Platform = xQueueCreate(2, sizeof(PLATFORM_t));

    // update platform location every 25ms
    ece353_T32_2_Interrupt_Ms(25);

}

// this task handles all platform generation and rendering
void Task_Platform(void *pvParameters) {

    uint32_t ulEventToProcess;
    //PLATFORM_t platform;
    BaseType_t status;

    // adding 1 test platform for now
    platforms[0].type = NORMAL;
    platforms[0].id = 0;
    platforms[0].x = LCD_HORIZONTAL_MAX - 30;
    platforms[0].y = LCD_VERTICAL_MAX - 30;

    platforms[1].type = NORMAL;
    platforms[1].id = 0;
    platforms[1].x = LCD_HORIZONTAL_MAX - 75;
    platforms[1].y = LCD_VERTICAL_MAX - 60;

    // draw platform initially
    lcd_draw_image(
            platforms[0].x,
            platforms[0].y,
            platformWidthPixels,
            platformHeightPixels,
            platform_bitmap,
            LCD_COLOR_YELLOW,
            LCD_COLOR_BLACK
    );

    // draw platform initially
    lcd_draw_image(
            platforms[1].x,
            platforms[1].y,
            platformWidthPixels,
            platformHeightPixels,
            platform_bitmap,
            LCD_COLOR_YELLOW,
            LCD_COLOR_BLACK
    );

    while(1) {

        // wait until we get a task notification from the T32_INT2 ISR
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        status = xSemaphoreTake(Sem_LCD_Draw, portMAX_DELAY);

        lcd_draw_image(
                platforms[0].x,
                platforms[0].y,
                platformWidthPixels,
                platformHeightPixels,
                platform_bitmap,
                LCD_COLOR_YELLOW,
                LCD_COLOR_BLACK
        );

        // draw platform initially
        lcd_draw_image(
                platforms[1].x,
                platforms[1].y,
                platformWidthPixels,
                platformHeightPixels,
                platform_bitmap,
                LCD_COLOR_YELLOW,
                LCD_COLOR_BLACK
        );

        xSemaphoreGive(Sem_LCD_Draw);

        // necessary task delay
        vTaskDelay(pdMS_TO_TICKS(25));

    }

}


void T32_INT2_IRQHandler() {

    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

    vTaskNotifyGiveFromISR(
            Task_Platform_Handle,
            &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    // DONT FORGET TO CLEAR THE INTERRUPT
    TIMER32_2->INTCLR = BIT0;

}

