/*
 * task_accelerometer.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_Accelerometer_Handle;
TaskHandle_t Task_Accelerometer_Timer_Handle;

volatile uint32_t ACCELEROMETER_X_DIR = 0;

/******************************************************************************
* This function initialized the functionality of the accelerometer on the MKII
******************************************************************************/
void accel_init(void) {

    // configure the X direction as an analog input P6.1 (A14)
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP;

    // configure CTL1 to return 14-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_3;

    // associate the ACC_XOUT signal with MEM[0]
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_14;

    // enable interrupts after a value has been written into MEM[0]
    ADC14->IER0 |= ADC14_IER0_IE0;

    // enable ADC interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // TODO possibly tweak interrupt priority if needed
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC on
    ADC14->CTL0 |= ADC14_CTL0_ON;

    // LEDS for fun
    ece353_MKII_RGB_IO_Init(false);

}

/******************************************************************************
* Used to start an ADC14 conversion
******************************************************************************/
void Task_Accelerometer_Timer(void *pvParameters) {

    while (1) {

        // start an ADC conversion
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // delay 5ms
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/******************************************************************************
* Bottom Half | Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters) {

    uint32_t ulEventToProcess;
    BUCKY_MSG_t bucky_msg;
    BaseType_t status;

    while (1) {

        // wait until we get a task notification from the ADC14 ISR
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


        if (ACCELEROMETER_X_DIR > VOLT_1P7) {        /* RIGHT TILT */

            // ece353_MKII_RGB_LED(true, false, false);        // red

            bucky_msg.cmd = BUCKY_RIGHT;
            bucky_msg.speed = 1;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        } else if (ACCELEROMETER_X_DIR < VOLT_1P6) {  /* LEFT TILT */

            // ece353_MKII_RGB_LED(false, false, true);        // green

            bucky_msg.cmd = BUCKY_LEFT;
            bucky_msg.speed = 1;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);


        } else {                                            /* CENTER */

            // ece353_MKII_RGB_LED(false, true, false);        // blue

            bucky_msg.cmd = BUCKY_CENTER;
            bucky_msg.speed = 1;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/******************************************************************************
* Bottom Half | ADC14 IRQHandler
******************************************************************************/
void ADC14_IRQHandler(void) {

    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

    // read the value and clear the interrupt
    ACCELEROMETER_X_DIR = ADC14->MEM[0];

    // send a task notification to Task_Accelerometer_Bottom_Half
    vTaskNotifyGiveFromISR(
            Task_Accelerometer_Handle,
            &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

