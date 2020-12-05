/*
 * task_accelerometer.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_ACCELEROMETER_H_
#define TASK_ACCELEROMETER_H_

#include <main.h>

extern TaskHandle_t Task_Accelerometer_Handle;

void Task_Accelerometer(void *pvParameters);

#endif /* TASK_ACCELEROMETER_H_ */
