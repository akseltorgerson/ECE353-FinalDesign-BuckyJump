/*
 * task_buzzer.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Buzzer_Handle;

/******************************************************************************
 * This function will initialize the buzzer on the MKII
 ******************************************************************************/
void buzzer_init(void) {

    /* Initialize the buzzer on the MKII */
    ece353_MKII_Buzzer_Init(0);

}

/******************************************************************************
 * This task manages end of game screen and song that plays through the buzzer
 ******************************************************************************/
void Task_Buzzer(void *pvParameters) {

    BaseType_t status;

    /* Boolean to keep track if a song has been played already */
    bool songPlayed = false;

    /* This is the highest priority task, suspend it until needed */
    vTaskSuspend(Task_Buzzer_Handle);

    while (1) {

        /* Suspend tasks that would interfere */
        vTaskSuspend(Task_Accelerometer_Handle);
        vTaskSuspend(Task_Bucky_Handle);
        vTaskSuspend(Task_Platform_Handle);
        vTaskSuspend(Task_LightSensor_Handle);

        /* Draw game over splash screen */
        lcd_draw_image(
                LCD_HORIZONTAL_MAX / 2,
                LCD_VERTICAL_MAX / 2,
                splashScreenWidthPixels,
                splashScreenHeightPixels,
                gameover_Bitmap,
                LCD_COLOR_RED,
                LCD_COLOR_BLACK
        );

        /* If the song hasn't been played yet */
        if (!songPlayed) {

            /* Play the song */
            music_play_song();
            songPlayed = true;

        }

        /* Here is where we wait until BUTTON1 has been pressed */
        if (BUTTON1_PRESSED) {

            BUTTON1_PRESSED = false;
            songPlayed = false;

            /* Draw a black screen to "reset" the LCD */
            lcd_draw_image(
                    LCD_HORIZONTAL_MAX / 2,
                    LCD_VERTICAL_MAX / 2,
                    splashScreenWidthPixels,
                    splashScreenHeightPixels,
                    gameover_Bitmap,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
            );

            /* Resume all previously disabled tasks */
            vTaskResume(Task_Accelerometer_Handle);
            vTaskResume(Task_Bucky_Handle);
            vTaskResume(Task_Platform_Handle);
            vTaskResume(Task_LightSensor_Handle);

            /* Suspend this task so it does not interrupt any of the other tasks */
            vTaskSuspend(Task_Buzzer_Handle);

        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


