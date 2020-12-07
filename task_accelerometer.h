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

/* tilt left threshold */
#define VOLT_1P6    7943        // 1.60 / (3.3 / 16383)
#define VOLT_1P55   7695        // 1.55
#define VOLT_1P5    7447        // 1.50
#define VOLT_1P45   7199        // 1.45

/* tilt right threshold */
#define VOLT_1P7    8440        // 1.7 / (3.3 / 16383)
#define VOLT_1P75   8688        // 1.75
#define VOLT_1P8    8936        // 1.8
#define VOLT_1P85   9184        // 1.85

#define BUCKY_SPEED_DAMPENING   .015

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
