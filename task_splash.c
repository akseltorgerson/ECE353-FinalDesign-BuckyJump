/*
 * task_splash.c
 *
 *  Created on: Dec 9, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Splash_Handle;


void Task_Splash(void *pvParameters) {

    vTaskSuspend(Task_Accelerometer_Handle);
    vTaskSuspend(Task_Bucky_Handle);
    vTaskSuspend(Task_Platform_Handle);

    BaseType_t ulEventToProcess;
    BaseType_t status;

    bool dir = true;
    uint8_t i = 40;

    lcd_draw_image(
            LCD_HORIZONTAL_MAX / 2,
            LCD_VERTICAL_MAX / 2,
            splashScreenWidthPixels,
            splashScreenHeightPixels,
            splashScreen0_Bitmap,
            LCD_COLOR_RED,
            LCD_COLOR_BLACK
    );

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

        if (dir) {
            i++;
        } else {
            i--;
        }

        if (dir && i >= 80) {
            dir = false;
        } else if (!dir && i <= 40) {
            dir = true;
        }


        if (BUTTON1_PRESSED) {

            BUTTON1_PRESSED = false;

            lcd_draw_image(
                    LCD_HORIZONTAL_MAX / 2,
                    LCD_VERTICAL_MAX / 2,
                    splashScreenWidthPixels,
                    splashScreenHeightPixels,
                    splashScreen0_Bitmap,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
            );

        // wait until we get a task notification from the T32_INT1 ISR
        //ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            vTaskResume(Task_Accelerometer_Handle);
            vTaskResume(Task_Bucky_Handle);
            vTaskResume(Task_Platform_Handle);

            vTaskSuspend(Task_Splash_Handle);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}






