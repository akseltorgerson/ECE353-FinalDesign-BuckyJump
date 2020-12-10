/*
 * task_splash.h
 *
 *  Created on: Dec 9, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_SPLASH_H_
#define TASK_SPLASH_H_

#include <main.h>

extern TaskHandle_t Task_Splash_Handle;

/******************************************************************************
 * This task manages the start game splash screen
 ******************************************************************************/
void Task_Splash(void *pvParameters);

#endif /* TASK_SPLASH_H_ */
