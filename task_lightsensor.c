/*
 * task_lightsensor.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_LightSensor_Handle;
uint16_t LIGHT_VAL;

void light_init(void) {

    // init i2c bus
    i2c_init();

    // init light sensor
    // i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, OPT3001_RST);

    // Program the config register to power up, covert every 800ms, and be continuous
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, 0xC400);

}

void Task_LightSensor_Bottom_Half(void *pvParameters) {

    BaseType_t ulEventToProcess;
    BaseType_t status;
    BUCKY_MSG_t bucky_msg;

    while (1) {

        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        OPT3001_read_light();

        if (LIGHT_VAL < 80) {

            // send dark mode
            bucky_msg.cmd = BUCKY_COLOR;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        } else {

            // light mode
            bucky_msg.cmd = BUCKY_COLOR;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

            }

        vTaskDelay(pdMS_TO_TICKS(50));
        }

}

void OPT3001_read_light(void) {

    // read from the register, if the conversion is ready then return a value
    LIGHT_VAL = i2c_read_16(I2C_LIGHT_ADDR, I2C_LIGHT_RESULT);

}



