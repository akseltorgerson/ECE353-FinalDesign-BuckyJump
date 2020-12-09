/*
 * task_joystick.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel
 */

#ifndef TASK_LIGHTSENSOR_H_
#define TASK_LIGHTSENSOR_H_

#include <main.h>

#define I2C_LIGHT_ADDR      0x44

#define I2C_LIGHT_RESULT    0x00
#define I2C_LIGHT_CONFIG    0x01
#define I2C_LIGHT_LO_LIM    0x02
#define I2C_LIGHT_HI_LIM    0x03
#define I2C_LIGHT_MAN_ID    0x7E
#define I2C_LIGHT_DEV_ID    0x7F

/* Configuration registers */
#define OPT3001_RST         0xC810 /* not a mask */
#define OPT3001_FLG_LOW     0x0020 // low flag
#define OPT3001_FLG_HIGH    0x0040 // high flag
#define OPT3001_CVN800MS    0x0800 // convert every 800ms
#define OPT3001_MD_CONT     0x0600 // we want continuous mode
#define OPT3001_CNV_RDY     0x0080 // conversion ready when this bit is set

extern TaskHandle_t Task_LightSensor_Handle;
extern uint16_t LIGHT_VAL;

void light_init(void);

void Task_LightSensor_Bottom_Half(void *pvParameters);

void OPT3001_read_light(void);

#endif /* TASK_LIGHTSENSOR_H_ */
