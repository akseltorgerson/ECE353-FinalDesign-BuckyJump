/*
 * task_bucky.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Bucky_Handle;
TaskHandle_t Task_Jump_Handle;
QueueHandle_t Queue_Bucky;
SemaphoreHandle_t Sem_LCD_Draw;

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
 * or right, and the button S2 makes Bucky jump. The button S2 will send
 * a jump command to the queue,
 ******************************************************************************/
void Task_Bucky(void *pvParameters) {

    uint8_t buckyX = 64;
    uint8_t buckyY = LCD_HORIZONTAL_MAX - (buckySmallHeightPixels / 2);

    uint8_t delayMS = BASE_DELAY;

    BUCKY_MSG_t bucky_msg;
    BaseType_t status;

    int i;

    int height = 0;
    bool jump = false;
    bool falling = false;
    bool standing = true;
    bool platHit = false;

    // draw Bucky initially
    lcd_draw_image(
            buckyX,
            buckyY,
            buckySmallWidthPixels,
            buckySmallHeightPixels,
            buckyCenterSmall_bitmap,
            LCD_COLOR_RED,
            LCD_COLOR_BLACK
    );

    while(1) {

        status = xQueueReceive(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        if (bucky_msg.cmd == BUCKY_LEFT) {

            if (buckyX - 1 >= (buckySmallWidthPixels / 2)) {


                buckyX--;
                //buckyDir[] = buckyLeftSmall_bitmap;
                delayMS = bucky_msg.speed;
            }

        } else if (bucky_msg.cmd == BUCKY_RIGHT) {

            if (buckyX + 1 <= (LCD_HORIZONTAL_MAX - (buckySmallWidthPixels / 2))) {

                buckyX++;
                //buckyDir[] = buckyRightSmall_bitmap[];
                delayMS = bucky_msg.speed;
            }

        } else if (bucky_msg.cmd == BUCKY_CENTER) {

            delayMS = bucky_msg.speed;

        } else if (bucky_msg.cmd == BUCKY_JUMP && standing) {

            jump = true;
            falling = false;
            standing = false;
            height = 0;

        }

        if (jump) {             // if we are jumping

            if (height >= 50) {

                jump = false;
                falling = true;
                height = 0;

            } else {

                height++;

                if (buckyY - 1 >= (buckySmallHeightPixels / 2)) {

                    buckyY--;
                } else {

                    jump = false;
                    falling = true;
                    height = 0;

                }
            }

        } else {            // if were not jumping we are standing or falling or both!

            platHit = false;

            // scan all platforms
            for (i = 0; i < 2; i++ ) {

                // if we are in the hitbox range for any of the platforms
                if      ((buckyX + (buckySmallWidthPixels  / 4) >= platforms[i].x - (platformWidthPixels  / 2)) &&
                        (buckyX - (buckySmallWidthPixels  / 4) <= platforms[i].x + (platformWidthPixels  / 2)) &&
                        (buckyY + (buckySmallHeightPixels / 2) == platforms[i].y - (platformHeightPixels / 2))) {

                    // we hit a platform
                    platHit = true;
                }
            }

            // check if we are on the bottom of the screen
            if (buckyY + 1 >= LCD_HORIZONTAL_MAX - (buckySmallHeightPixels / 2)) {

                falling = false;
                standing = true;

            } else if (falling && platHit) {

                // we hit a platform or the bottom of the screen
                falling = false;
                standing = true;

            } else if (standing && !platHit) {

                falling = true;
                standing = false;

            }

            if (falling) {

                buckyY++;

            }
        }

        status = xSemaphoreTake(Sem_LCD_Draw, portMAX_DELAY);


        if (jump) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyJumpSmall_bitmap,
                    LCD_COLOR_RED,
                    LCD_COLOR_BLACK
            );

        } else if (bucky_msg.cmd == BUCKY_RIGHT) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyRightSmall_bitmap,
                    LCD_COLOR_RED,
                    LCD_COLOR_BLACK
            );

        } else if (bucky_msg.cmd == BUCKY_LEFT) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyLeftSmall_bitmap,
                    LCD_COLOR_RED,
                    LCD_COLOR_BLACK
            );

        } else {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyCenterSmall_bitmap,
                    LCD_COLOR_RED,
                    LCD_COLOR_BLACK
            );

        }

        xSemaphoreGive(Sem_LCD_Draw);

        // necessary task delay, default is 25ms
        vTaskDelay(pdMS_TO_TICKS(delayMS));
    }

}






