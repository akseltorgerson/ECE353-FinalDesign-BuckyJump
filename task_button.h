/*
 * task_button.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_BUTTON_H_
#define TASK_BUTTON_H_

#include <main.h>

extern TaskHandle_t Task_Button_Handle;

volatile extern bool BUTTON1_PRESSED;
volatile extern bool BUTTON2_PRESSED;

void button_init(void);

void Task_Button(void *pvParameters);

#endif /* TASK_BUTTON_H_ */
