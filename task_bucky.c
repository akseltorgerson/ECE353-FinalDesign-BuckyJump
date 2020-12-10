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

    /* Variables used to store game state info */
    int i;
    int buckyColor = LCD_COLOR_RED;
    int height = 0;
    bool jump = false;
    bool falling = false;
    bool standing = true;
    bool platHit = false;
    bool gameStart = false;

    while(1) {

        /* Wait until a command is sent to the queue */
        status = xQueueReceive(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        if (bucky_msg.cmd == BUCKY_LEFT) {                                          // LEFT COMMAND

            /* Boundary detection */
            if (buckyX - 1 >= (buckySmallWidthPixels / 2)) {

                /* Move Bucky in the respective direction */
                buckyX--;

                /* Delay is controlled by the speed field of BUCKY_MSG_t */
                delayMS = bucky_msg.speed;
            }

        } else if (bucky_msg.cmd == BUCKY_RIGHT) {                                  // RIGHT COMMAND

            /* Boundary detection */
            if (buckyX + 1 <= (LCD_HORIZONTAL_MAX - (buckySmallWidthPixels / 2))) {

                /* Move Bucky in the respective direction */
                buckyX++;

                /* Delay is controlled by the speed field of BUCKY_MSG_t */
                delayMS = bucky_msg.speed;
            }

        } else if (bucky_msg.cmd == BUCKY_CENTER) {                                 // CENTER

            /* Don't need to move bucky or check for bounds, just set delay */
            delayMS = bucky_msg.speed;

        } else if (bucky_msg.cmd == BUCKY_COLOR) {                                  // COLOR (for dark mode)

            /* Here the speed field is used to determine if we should be in light or dark mode */
            if (bucky_msg.speed) {

                /* If the speed == 1, dark mode */
                buckyColor = LCD_COLOR_GRAY;

            } else {

                /* Else light mode */
                buckyColor = LCD_COLOR_RED;

            }

        } else if (bucky_msg.cmd == BUCKY_JUMP && standing) {                       // JUMP COMMAND

            /* Jump commands will get sent every time the jump button is pressed,  */
            /* but Bucky needs to be standing in order for a new jump to process.  */
            jump = true;
            falling = false;
            standing = false;
            height = 0;
            gameStart = true;

        }

        /*********************** ALL CODE BELOW COULD USE MAJOR OPTIMIZATIONS *********************/

        /* If we are jumping */
        if (jump) {

            /* If we hit max jump height */
            if (height >= 50) {

                /* Stop jumping and start falling */
                jump = false;
                falling = true;
                height = 0;

            } else {

                /* Increase the height by 1 */
                height++;

                /* If we below the top of the screen */
                if (buckyY - 1 >= (buckySmallHeightPixels / 2)) {

                    /* Move Bucky up by 1 */
                    buckyY--;

                /* If we hit the top of the screen */
                } else {

                    /* Stop jumping, start falling */
                    jump = false;
                    falling = true;
                    height = 0;

                }
            }

        /* If we are not jumping */
        } else {

            /* Keep track if we hit any platforms */
            platHit = false;

            /* Scan all platforms on the screen */
            for (i = 0; i < numPlat; i++ ) {

                /* If we are in the right hit box area */
                if      ((buckyX + (buckySmallWidthPixels  / 4) >= platforms[i].x - (platformWidthPixels  / 2)) &&
                        (buckyX - (buckySmallWidthPixels  / 4) <= platforms[i].x + (platformWidthPixels  / 2)) &&
                        (buckyY + (buckySmallHeightPixels / 2) == platforms[i].y - (platformHeightPixels / 2))) {

                    /* We are on a platform */
                    platHit = true;
                }
            }

            /* Check if we are at the bottom of the screen */
            if (buckyY + 1 >= LCD_HORIZONTAL_MAX - (buckySmallHeightPixels / 2)) {

                falling = false;
                standing = true;

                /* If the game has started already */
                if (gameStart) {

                    /* Reset all variables used in the game preemptively */
                    height = 0;
                    jump = false;
                    falling = false;
                    standing = true;
                    platHit = false;
                    gameStart = false;

                    /* Resume Task_Buzzer */
                    /* This deals with the gameover screen and playing the music */
                    vTaskResume(Task_Buzzer_Handle);
                }

            /* If were not at the bottom of the screen, check and see if were falling and hit a platform */
            } else if (falling && platHit) {

                /* We hit a platform */
                falling = false;
                standing = true;


            /* If were standing and we did not hit a platform */
            } else if (standing && !platHit) {

                falling = true;
                standing = false;

            }

            /* If we hit a platform */
            if (platHit) {

                standing = true;
            }

            /* If we are still falling after all the calculations above */
            if (falling) {

                buckyY++;

            }
        }

        /* Use a semaphore to allow access to writing to the LCD */
        status = xSemaphoreTake(Sem_LCD_Draw, portMAX_DELAY);

        /* Draw different sprites (animation) depending on Bucky's movement */
        if (jump) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyJumpSmall_bitmap,
                    buckyColor,
                    LCD_COLOR_BLACK
            );

        } else if (bucky_msg.cmd == BUCKY_RIGHT) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyRightSmall_bitmap,
                    buckyColor,
                    LCD_COLOR_BLACK
            );

        } else if (bucky_msg.cmd == BUCKY_LEFT) {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyLeftSmall_bitmap,
                    buckyColor,
                    LCD_COLOR_BLACK
            );

        } else {

            lcd_draw_image(
                    buckyX,
                    buckyY,
                    buckySmallWidthPixels,
                    buckySmallHeightPixels,
                    buckyCenterSmall_bitmap,
                    buckyColor,
                    LCD_COLOR_BLACK
            );

        }

        /* Release the semaphore back */
        xSemaphoreGive(Sem_LCD_Draw);

        /* Task delay for delayMS */
        vTaskDelay(pdMS_TO_TICKS(delayMS));
    }

}






