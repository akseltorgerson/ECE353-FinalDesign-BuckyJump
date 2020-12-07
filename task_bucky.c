/*
 * task_bucky.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

#define BUCKY_QUEUE_LEN 2

TaskHandle_t Task_Bucky_Handle;
TaskHandle_t Task_Jump_Handle;
QueueHandle_t Queue_Bucky;

/******************************************************************************
 * This function will initialize Queue_Bucky and initialize the LCD
 ******************************************************************************/
void bucky_init(void) {

    // init the queue used to send Bucky commands
    Queue_Bucky = xQueueCreate(BUCKY_QUEUE_LEN, sizeof(BUCKY_MSG_t));

    // init the LCD
    Crystalfontz128x128_Init();

}

/******************************************************************************
 * This task manages the movement of Bucky. The accelerometer moves Bucky left
 * or right, and the button S2 makes Bucky jump.
 ******************************************************************************/
void Task_Bucky(void *pvParameters) {

    uint8_t buckyX = 64;
    uint8_t buckyY = LCD_HORIZONTAL_MAX - (buckyHeightPixels / 2);

    uint8_t delayMS = BASE_DELAY;

    BUCKY_MSG_t bucky_msg;
    BaseType_t status;

    int height = 0;
    bool jump = false;
    bool up = true;

    // draw Bucky initially
    lcd_draw_image(
            buckyX,
            buckyY,
            buckyWidthPixels,
            buckyHeightPixels,
            buckyRight_bitmap,
            LCD_COLOR_RED,
            LCD_COLOR_BLACK
    );

    while(1) {

        status = xQueueReceive(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        if (bucky_msg.cmd == BUCKY_LEFT) {

            // if (buckyX - 1 <= (buckyWidthPixels / 2)) break;
            buckyX--;
            delayMS = bucky_msg.speed;

        } else if (bucky_msg.cmd == BUCKY_RIGHT) {

            // if (buckyX + 1 >= (LCD_VERTICAL_MAX - (buckyWidthPixels / 2))) break;
            buckyX++;
            delayMS = bucky_msg.speed;

        } else if (bucky_msg.cmd == BUCKY_CENTER) {

            delayMS = bucky_msg.speed;

        } else if (bucky_msg.cmd == BUCKY_JUMP && !jump) {

            jump = true;

        }

        if (jump) {
            if (up) {
                buckyY--;
                if (height >= 50) {
                    up = false;
                } else {
                    height++;
                }
            } else {
                buckyY++;
                if (height <= 0 ) {
                    up = true;
                    jump = false;
                    height = 0;
                } else {
                    height--;
                }
            }
        }

        lcd_draw_image(
                buckyX,
                buckyY,
                buckyWidthPixels,
                buckyHeightPixels,
                buckyRight_bitmap,
                LCD_COLOR_RED,
                LCD_COLOR_BLACK
        );

        // necessary task delay, default is 25ms
        vTaskDelay(pdMS_TO_TICKS(delayMS));
    }

}






