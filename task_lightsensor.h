/*
 * task_joystick.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel
 */

#ifndef TASK_LIGHTSENSOR_H_
#define TASK_LIGHTSENSOR_H_

#include <main.h>

extern TaskHandle_t Task_LightSensor_Handle;

void Task_LightSensor(void *pvParameters);

#endif /* TASK_LIGHTSENSOR_H_ */
