/*
 * task_accelerometer.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_ACCELEROMETER_H_
#define TASK_ACCELEROMETER_H_

#include "main.h"

/* "flat" voltage reading */
#define VOLT_1P65   8192      // 1.65 / (3.3 / 16383)

#define BUCKY_SPEED_DAMPENING   .01

extern TaskHandle_t Task_Accelerometer_Handle;
extern TaskHandle_t Task_Accelerometer_Timer_Handle;

/******************************************************************************
* This function initialized the functionality of the accelerometer on the MKII
******************************************************************************/
void accel_init(void);

/******************************************************************************
* Used to start an ADC14 Conversion
******************************************************************************/
void Task_Accelerometer_Timer(void *pvParameters);

/******************************************************************************
* Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters);

#endif /* TASK_ACCELEROMETER_H_ */
