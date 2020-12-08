/*
 * task_platform.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Aksel
 */

#ifndef TASK_PLATFORM_H_
#define TASK_PLATFORM_H_

#include <main.h>

#define BASE_PLATFORM_DELAY 25;
#define PLATFORM_QUEUE_LEN  2;

typedef enum {
    NORMAL,
    BONUS,
    FAKE,
    // other types of platforms
} PLATFORM_TYPE_t;

typedef struct {
    PLATFORM_TYPE_t type;
    uint8_t         id;
    uint8_t         x;
    uint8_t         y;
} PLATFORM_t ;

extern TaskHandle_t Task_Platform_Handle;
extern QueueHandle_t Queue_Platform;
extern PLATFORM_t platforms[];
extern uint8_t numPlat;


/******************************************************************************
* * This function will initialize Queue_Platform and other things needed
******************************************************************************/
void platform_init(void);

/******************************************************************************
 * This task manages the movement of the platforms.
 ******************************************************************************/
void Task_Platform(void *pvParameters);


#endif /* TASK_PLATFORM_H_ */
