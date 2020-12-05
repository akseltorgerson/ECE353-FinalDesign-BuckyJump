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

void Task_Buzzer(void *pvParameters);

#endif /* TASK_BUZZER_H_ */
