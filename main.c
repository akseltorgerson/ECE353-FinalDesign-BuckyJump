/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * main.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#include "main.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    /* Initialization before scheduler */
    bucky_init();
    accel_init();
    button_init();
    platform_init();

    __enable_irq();

    /* Task initialization */

    // TODO Fix side hit boxes
    // TODO Add button tasks

    // TODO Add random element that generates platforms
    // TODO Get bitmap for platforms and coins.

    // TODO Start working on jumping on stationary platforms

    // TODO Make platforms fall
    // TODO Make Bucky be able to jump from them

    // TODO TODO make screen scrool up, in doodle jump fashion (if possible)
    // TODO keep track of score
    // TODO when you hit the bottom of the screen you die and your score is shown

    xTaskCreate
        (   Task_Accelerometer_Timer,
            "Task_Accelerometer",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            &Task_Accelerometer_Timer_Handle
        );

    xTaskCreate
         (   Task_Accelerometer_Bottom_Half,
             "Task_Accelerometer",
             configMINIMAL_STACK_SIZE,
             NULL,
             3,
             &Task_Accelerometer_Handle
         );

    xTaskCreate
         (   Task_Bucky,
             "Task_Bucky",
             configMINIMAL_STACK_SIZE,
             NULL,
             1,
             &Task_Bucky_Handle
         );

    xTaskCreate
         (   Task_Platform,
             "Task_Platform",
             configMINIMAL_STACK_SIZE,
             NULL,
             1,
             &Task_Platform_Handle
         );

    xTaskCreate
         (   Task_Button_Bottom_Half,
             "Task_Button",
             configMINIMAL_STACK_SIZE,
             NULL,
             3,
             &Task_Button_Handle
         );

    xTaskCreate
         (   Task_Buzzer,
             "Task_Buzzer",
             configMINIMAL_STACK_SIZE,
             NULL,
             1,
             &Task_Buzzer_Handle
         );

    xTaskCreate
         (   Task_LightSensor,
             "Task_LightSensor",
             configMINIMAL_STACK_SIZE,
             NULL,
             1,
             &Task_LightSensor_Handle
         );

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
