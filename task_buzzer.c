/*
 * task_buzzer.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Buzzer_Handle;

bool songPlayed = false;

void Task_Buzzer(void *pvParameters) {

    BaseType_t status;

    vTaskSuspend(Task_Buzzer_Handle);

    while (1) {

        vTaskSuspend(Task_Accelerometer_Handle);
        vTaskSuspend(Task_Bucky_Handle);
        vTaskSuspend(Task_Platform_Handle);
        vTaskSuspend(Task_LightSensor_Handle);

        lcd_draw_image(
                LCD_HORIZONTAL_MAX / 2,
                LCD_VERTICAL_MAX / 2,
                splashScreenWidthPixels,
                splashScreenHeightPixels,
                gameover_Bitmap,
                LCD_COLOR_RED,
                LCD_COLOR_BLACK
        );

        if (!songPlayed) {
            music_play_song();
            songPlayed = true;
        }

        if (BUTTON1_PRESSED) {

            BUTTON1_PRESSED = false;
            songPlayed = false;

            lcd_draw_image(
                    LCD_HORIZONTAL_MAX / 2,
                    LCD_VERTICAL_MAX / 2,
                    splashScreenWidthPixels,
                    splashScreenHeightPixels,
                    gameover_Bitmap,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
            );

            vTaskResume(Task_Accelerometer_Handle);
            vTaskResume(Task_Bucky_Handle);
            vTaskResume(Task_Platform_Handle);
            vTaskResume(Task_LightSensor_Handle);

            vTaskSuspend(Task_Buzzer_Handle);

        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


