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
extern uint8_t numPlat = 6;
time_t t;

void platform_init(void) {

    // init platform queue
    // not used right now, just here for testing
    Queue_Platform = xQueueCreate(2, sizeof(PLATFORM_t));

    // update platform location every 25ms
    ece353_T32_2_Interrupt_Ms(50);

    // init random number generator
    srand((unsigned) &(t));

}

// this task handles all platform generation and rendering
void Task_Platform(void *pvParameters) {

    uint32_t ulEventToProcess;
    //PLATFORM_t platform;
    BaseType_t status;

    uint8_t numCol = 3;

    int i;

    // create platforms
    for (i = 0; i < numPlat; i++) {

        platforms[i].type = NORMAL;
        platforms[i].id = i;
        platforms[i].x = ((LCD_HORIZONTAL_MAX - (((i % numCol) + 1) * (LCD_HORIZONTAL_MAX / (numCol + 1)))));
        platforms[i].y = (LCD_VERTICAL_MAX - ((i  +  1) * (255 / numPlat)));

    }

    while(1) {

        // wait until we get a task notification from the T32_INT2 ISR
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        status = xSemaphoreTake(Sem_LCD_Draw, portMAX_DELAY);

        for (i = 0; i < numPlat; i++) {

            // if the platform is about to be off the screen.
            if ((platforms[i].y + 1) == (LCD_VERTICAL_MAX - (platformHeightPixels / 2))) {

                // draw a black platform over it
                lcd_draw_image(
                        platforms[i].x,
                        platforms[i].y,
                        platformWidthPixels,
                        platformHeightPixels,
                        platform_bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                // set its value just off the bottom of the screen
                platforms[i].y += platformHeightPixels;

                // give it a random x value
                platforms[i].x = ((LCD_HORIZONTAL_MAX - (((rand() % numCol) + 1) * (LCD_HORIZONTAL_MAX / (numCol + 1)))));

            }

            lcd_draw_image(
                    platforms[i].x,
                    platforms[i].y++,
                    platformWidthPixels,
                    platformHeightPixels,
                    platform_bitmap,
                    LCD_COLOR_YELLOW,
                    LCD_COLOR_BLACK
            );
        }

        xSemaphoreGive(Sem_LCD_Draw);

        // necessary task delay
        vTaskDelay(pdMS_TO_TICKS(10));

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

