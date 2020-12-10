/*
 * task_splash.c
 *
 *  Created on: Dec 9, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Splash_Handle;

/******************************************************************************
 * This task manages the start game splash screen
 ******************************************************************************/
void Task_Splash(void *pvParameters) {

    /* Temporarily suspend interfering tasks */
    vTaskSuspend(Task_Accelerometer_Handle);
    vTaskSuspend(Task_Bucky_Handle);
    vTaskSuspend(Task_Platform_Handle);

    BaseType_t ulEventToProcess;
    BaseType_t status;

    /* Variables to control start screen animation */
    bool dir = true;
    uint8_t i = 40;

    /* Draw the splash screen */
    lcd_draw_image(
            LCD_HORIZONTAL_MAX / 2,
            LCD_VERTICAL_MAX / 2,
            splashScreenWidthPixels,
            splashScreenHeightPixels,
            splashScreen0_Bitmap,
            LCD_COLOR_RED,
            LCD_COLOR_BLACK
    );

    /* Draw Bucky jumping up and down */
    while (1) {

        lcd_draw_image(
                LCD_HORIZONTAL_MAX / 2,
                LCD_VERTICAL_MAX - i,
                buckySmallWidthPixels,
                buckySmallHeightPixels,
                buckyRightSmall_bitmap,
                LCD_COLOR_RED,
                LCD_COLOR_BLACK
        );

        /* If Bucky is going up */
        if (dir) {

            /* Add one to his height */
            i++;

        } else {

            /* Subtract one to his height */
            i--;

        }

        /* If he gets to a certain height, start making him fall */
        if (dir && i >= 80) {

            dir = false;

        } else if (!dir && i <= 40) {

            dir = true;

        }

        /* If button 1 has been pressed */
        if (BUTTON1_PRESSED) {

            BUTTON1_PRESSED = false;

            /* Draw a black image to "reset" the LCD */
            lcd_draw_image(
                    LCD_HORIZONTAL_MAX / 2,
                    LCD_VERTICAL_MAX / 2,
                    splashScreenWidthPixels,
                    splashScreenHeightPixels,
                    splashScreen0_Bitmap,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
            );

            /* Resume all important tasks */
            vTaskResume(Task_Accelerometer_Handle);
            vTaskResume(Task_Bucky_Handle);
            vTaskResume(Task_Platform_Handle);

            /* Indefinitely suspend this task */
            vTaskSuspend(Task_Splash_Handle);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}






