/*
 * task_lightsensor.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include <main.h>

TaskHandle_t Task_LightSensor_Handle;

void light_init(void) {

    // init i2c bus
    i2c_init();

    // init light sensor
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, OPT3001_RST | OPT3001_CVN800MS | OPT3001_MD_CONT);

    // Program the config register to power up, covert every 800ms, and be continuous
    // i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, 0xC410);

}

void Task_LightSensor_Bottom_Half(void *pvParameters) {

    BaseType_t ulEventToProcess;
    BaseType_t status;
    BUCKY_MSG_t bucky_msg;
    uint16_t light_val;
    uint16_t lux;

    while (1) {

        ulEventToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        light_val = OPT3001_read_light();

        // convert to lux
        lux = (.01 * pow(2, (light_val & 0xF000)) * (light_val & 0x0FFF));

        if (lux < 100) {

            // send dark mode
            bucky_msg.cmd = BUCKY_COLOR;
            bucky_msg.speed = 1;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

        } else {

            // light mode
            bucky_msg.cmd = BUCKY_COLOR;
            bucky_msg.speed = 0;
            status = xQueueSendToBack(Queue_Bucky, &bucky_msg, portMAX_DELAY);

            }

        vTaskDelay(pdMS_TO_TICKS(10));
        }

}

uint16_t OPT3001_read_light(void) {

    uint16_t light_val = 0;
    // read from the register, if the conversion is ready then return a value
    light_val = i2c_read_16(I2C_LIGHT_ADDR, I2C_LIGHT_RESULT);

    return light_val;

}



