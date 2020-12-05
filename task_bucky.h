/*
 * task_bucky.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef TASK_BUCKY_H_
#define TASK_BUCKY_H_

#include <main.h>

typedef enum {
    BUCKY_LEFT,
    BUCKY_RIGHT,
    BUCKY_JUMP,
    BUCKY_SPEED
} BUCKY_CMD_t;

typedef struct {
    BUCKY_CMD_t cmd;
    uint8_t     value;
} BUCKY_MSG_t;

extern TaskHandle_t Task_Bucky_Handle;
extern QueueHandle_t Queue_Space_Ship;

/******************************************************************************
 * This function will initialize Queue_Bucky and initialize the LCD
 ******************************************************************************/
void bucky_init(void);

/******************************************************************************
 * This task manages the movement of Bucky. The accelerometer moves Bucky left
 * or right, and the button S2 makes Bucky jump.
 ******************************************************************************/
void Task_Bucky(void *pvParameters);

#endif /* TASK_BUCKY_H_ */
