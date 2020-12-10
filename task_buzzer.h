/*
 * task_buzzer.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_BUZZER_H_
#define TASK_BUZZER_H_

#include <main.h>

extern TaskHandle_t Task_Buzzer_Handle;

/******************************************************************************
 * This function will initialize the buzzer on the MKII
 ******************************************************************************/
void buzzer_init(void);

/******************************************************************************
 * This task manages end of game screen and song that plays through the buzzer
 ******************************************************************************/
void Task_Buzzer(void *pvParameters);

#endif /* TASK_BUZZER_H_ */
