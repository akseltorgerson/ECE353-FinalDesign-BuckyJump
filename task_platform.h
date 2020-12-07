/*
 * task_platform.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Aksel
 */

#ifndef TASK_PLATFORM_H_
#define TASK_PLATFORM_H_

#include <main.h>

extern TaskHandle_t Task_Platform_Handle;

/******************************************************************************
* * This function will initialize Queue_Platform and other things needed
******************************************************************************/
void platform_init(void);

/******************************************************************************
 * This task manages the movement of the platforms.
 ******************************************************************************/
void Task_Platform(void *pvParameters);


#endif /* TASK_PLATFORM_H_ */
