/*
 * task_lightsensor.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_LightSensor_Handle;

/******************************************************************************
* This function initializes the functionality of the I2C bus
******************************************************************************/
void light_init(void) {

    /* Initialize the I2C bus */
    i2c_init();

    /* Initialize the light sensor to perform how we want it to */
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, OPT3001_RST | OPT3001_CVN800MS | OPT3001_MD_CONT);

}

/******************************************************************************
* Examines the raw data read from the light sensor and sends a message to
* Task_Bucky accordingly
******************************************************************************/
void Task_LightSensor_Bottom_Half(void *pvParameters) {

    BaseType_t ulEventToProcess;
    BaseType_t status;
    BUCKY_MSG_t bucky_msg;
    uint16_t light_val;
    uint16_t lux;

    while (1) {

        /* Wait until we receive a task notification from T32_INT2_IRQHandler */
        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Read the raw data */
        light_val = OPT3001_read_light();

        /* Convert to a sensible LUX value */
        lux = (.01 * pow(2, (light_val & 0xF000)) * (light_val & 0x0FFF));

        /* If the lux is above 100 */
        if (lux < 100) {

            /* Send dark mode command */
            bucky_msg.cmd = BUCKY_COLOR;
            bucky_msg.speed = 1;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        } else {

            /* Send light mode command */
            bucky_msg.cmd = BUCKY_COLOR;
            bucky_msg.speed = 0;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        }

        vTaskDelay(pdMS_TO_TICKS(10));

    }
}

/******************************************************************************
* Reads and returns the raw data from the light sensor
******************************************************************************/
uint16_t OPT3001_read_light(void) {

    uint16_t light_val = 0;

    /* Read the value in the result register in the light sensor */
    light_val = i2c_read_16(I2C_LIGHT_ADDR, I2C_LIGHT_RESULT);

    /* Return it for conversion */
    return light_val;

}



