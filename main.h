/*
 * main.h
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Necessary library includes */
#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* Task Includes */
#include <task_accelerometer.h>
#include <task_bucky.h>
#include <task_button.h>
#include <task_buzzer.h>
#include <task_lightsensor.h>
#include <task_platform.h>

/* Other Includes */
#include <lcd.h>
#include <bitmaps.h>
#include <ece353.h>
#include <time.h>
#include <i2c.h>

#endif /* MAIN_H_ */
