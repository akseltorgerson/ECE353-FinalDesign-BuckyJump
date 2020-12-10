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

/******************************************************************************
 * This function will initialize the buttons and set T32_1 to interrupt every
 * 10ms
 ******************************************************************************/
void button_init(void);

/******************************************************************************
 * Bottom half of the button task. When it receives an interrupt from the ISR,
 * it will send a jump command to Task_Bucky.
 ******************************************************************************/
void Task_Button_Bottom_Half(void *pvParameters);

#endif /* TASK_BUTTON_H_ */
