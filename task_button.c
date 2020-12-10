/*
 * task_button.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Button_Handle;

/* Global variables to alert any tasks if button 1 or 2 has been pressed */
volatile extern bool BUTTON1_PRESSED = false;
volatile extern bool BUTTON2_PRESSED = false;

/******************************************************************************
 * This function will initialize the buttons and set T32_1 to interrupt every
 * 10ms
 ******************************************************************************/
void button_init() {

    /* Initialize S1 and S2 on the MKII */
    ece353_MKII_S1_Init();
    ece353_MKII_S2_Init();

    /* Initialize the T32_1 to generate an interrupt every 10ms */
    ece353_T32_1_Interrupt_Ms(10);

}

/******************************************************************************
 * Bottom half of the button task. When it receives an interrupt from the ISR,
 * it will send a jump command to Task_Bucky.
 ******************************************************************************/
void Task_Button_Bottom_Half(void *pvParameters) {

    uint32_t ulEventToProcess;
    BUCKY_MSG_t bucky_msg;
    BaseType_t status;

    while (1) {

        /* Wait until we get a task notification from the T32_INT1 ISR */
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Send a jump command to the queue */
        bucky_msg.cmd = BUCKY_JUMP;
        bucky_msg.speed = 1;
        status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        BUTTON2_PRESSED = false;

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/******************************************************************************
 * IRQ Handler for T32_1. Sends a task notification to Task_Button_Bottom_Half
 * if S2 has been de-bounced and pressed. If S1 has been de-bounced it sets the
 * global variable which is used later in other tasks (mutually exclusive).
 ******************************************************************************/
void T32_INT1_IRQHandler() {

    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

    if (!BUTTON2_PRESSED) {

        static uint8_t button2_state = 0x00;
        bool button2_pressed = ece353_MKII_S2();

        button2_state = button2_state << 1;

        if(button2_pressed) {

            button2_state |= 0x01;
        }

        /* 0111 1111 - Button has been pressed down for 70 consecutive ms */
        if(button2_state == 0x7F) {

            BUTTON2_PRESSED = true;

            /* Send a task notification to Task_Button_Bottom_Half */
            vTaskNotifyGiveFromISR(
                    Task_Button_Handle,
                    &xHigherPriorityTaskWoken
            );

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        }
    }


    if (!BUTTON1_PRESSED) {

        static uint8_t button1_state = 0x00;
        bool button1_pressed = ece353_MKII_S1();

        button1_state = button1_state << 1;

        if(button1_pressed) {

            button1_state |= 0x01;
        }

        if(button1_state == 0x7F) {

            BUTTON1_PRESSED = true;

        }
    }

    /* DONT FORGET TO CLEAR THE INTERRUPT */
    TIMER32_1->INTCLR = BIT0;

}

