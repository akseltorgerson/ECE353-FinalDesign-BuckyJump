/*
 * task_platform.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Platform_Handle;

/* Variables to store platform information */
PLATFORM_t platforms[10];
uint8_t numPlat = 6;
uint8_t numCol = 3;

/* Used for the random number generator seed */
time_t t;

/******************************************************************************
* * This function will initialize T32_2 and the random number generator
******************************************************************************/
void platform_init(void) {

    /* Configure the T32_2 to send an interrupt every 25ms */
    ece353_T32_2_Interrupt_Ms(25);

    /* Initialize the random number generator */
    srand((unsigned) &(t));

}

/******************************************************************************
 * This task manages the movement and generation of platforms
 ******************************************************************************/
void Task_Platform(void *pvParameters) {

    uint32_t ulEventToProcess;
    BaseType_t status;

    int i;

    /* Generate first iteration of platforms */
    for (i = 0; i < numPlat; i++) {

        platforms[i].type = NORMAL;
        platforms[i].id = i;
        platforms[i].x = ((LCD_HORIZONTAL_MAX - (((i % numCol) + 1) * (LCD_HORIZONTAL_MAX / (numCol + 1)))));
        platforms[i].y = (LCD_VERTICAL_MAX - ((i  +  1) * (255 / numPlat)));

    }

    while(1) {

        /* Wait until we get a task notification from the T32_INT2 ISR */
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Take the LCD semaphore */
        status = xSemaphoreTake(Sem_LCD_Draw, portMAX_DELAY);

        /* For every platform that exists */
        for (i = 0; i < numPlat; i++) {

            /* If the platform is about to be off the screen */
            if ((platforms[i].y + 1) == (LCD_VERTICAL_MAX - (platformHeightPixels / 2))) {

                /* Draw a black platform over it to rid the LCD artifacts */
                lcd_draw_image(
                        platforms[i].x,
                        platforms[i].y,
                        platformWidthPixels,
                        platformHeightPixels,
                        platform_bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                /* Set its value to just off screen below */
                platforms[i].y += platformHeightPixels;

                /* Give it a random x value */
                platforms[i].x = ((LCD_HORIZONTAL_MAX - (((rand() % numCol) + 1) * (LCD_HORIZONTAL_MAX / (numCol + 1)))));

            }

            /* Update the platforms location */
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

        /* Return the semaphore */
        xSemaphoreGive(Sem_LCD_Draw);

        vTaskDelay(pdMS_TO_TICKS(10));

    }
}

/******************************************************************************
 * IRQ Handler for T32_2. Sends a task notification to Task_LightSensor
 * every 200ms. Otherwise send a task notification to Task_Platform
 ******************************************************************************/
void T32_INT2_IRQHandler() {

    /* Use a simple shift variable to only wake LightSensor 1/8 times this ISR gets called */
    static uint8_t read_light = 0x00;
    read_light = read_light << 1;
    read_light |= 0x01;

    if (read_light == 0xFF) {

        BaseType_t xHigherPriorityTaskWoken = pdTRUE;

        vTaskNotifyGiveFromISR(
                Task_LightSensor_Handle,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        read_light = 0x00;

    /* Otherwise 7/8 other times send a task notification to Task_Platform */
    } else {

        BaseType_t xHigherPriorityTaskWoken = pdTRUE;

        vTaskNotifyGiveFromISR(
                Task_Platform_Handle,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    }

    /* DONT FORGET TO CLEAR THE INTERRUPT */
    TIMER32_2->INTCLR = BIT0;

}

