// Author: Joe Krachey
// Author: Aksel Torgerson
// Author: Matthew Kesler

#ifndef __ECE353_H__
#define __ECE353_H__

#include <stdbool.h>
#include <stdint.h>
#include "msp.h"

/*******************************************************
 * Initialize hardware resources used to control LED1
 ******************************************************/
void ece353_led1_init(void);

/*******************************************************
 * Initialize hardware resources used to control Button1
 ******************************************************/
void ece353_button1_init(void);

/*******************************************************
 * Turn LED1 ON/OFF
 *
 * Parameters
 *  on: if true, turn LED on
 *      if false, turn LED off
 ******************************************************/
void ece353_led1(bool on);

/*******************************************************
 * Returns if Button1 is currently pressed
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is not pressed
 ******************************************************/
bool ece353_button1(void);

//******************************************************
//******************************************************
// 02-ICE-Code-Organization
//******************************************************
//******************************************************

/*******************************************************
 * Initialize hardware resources used to control RGBLED
 ******************************************************/
void ece353_rgb_init(void);

/*******************************************************
 * Turn RGBLED ON/Off.
 *
 * Parameters
 *  red_on      :   if true,  turn RGBLED.RED on
 *                  if false, turn RGBLED.RED off
 *  green_on    :   if true,  turn RGBLED.GREEN on
 *                  if false, turn RGBLED.GREEN off
 *  blue_on     :   if true,  turn RGBLED.BLUE on
 *                  if false, turn RGBLED.BLUE off
 ******************************************************/
void ece353_rgb(bool red_on, bool green_on, bool blue_on);

/*******************************************************
 * Initialize hardware resources used to control Button2
 ******************************************************/
void ece353_button2_init(void);

/*******************************************************
 * Returns if Button2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button2 is pressed
 *      false   :   Button2 is NOT pressed
 ******************************************************/
bool ece353_button2(void);

/*******************************************************
 * Busy waits for a given number of clock cycles
 *
 * Parameters
 *      ticks   :   Number of ticks to wait
 *
 * Returns
 *      none
 ******************************************************/
void ece353_T32_1_wait(uint32_t ticks);

//*****************************************************************************
//*****************************************************************************
// ICE 03 - Timer32_1 Push Button Detection
//*****************************************************************************
//*****************************************************************************

/*****************************************************
 * Busy waits for 100mS and then returns.
 *
 * Timer32_1 MUST be configured as a 16-bit timer.
 * Assume that the MCU clock runs at 3MHz.  You will
 * need to use a pre-scalar in order to achieve a delay
 * of 100mS.
 *
 * Parameters
 *      ticks   :   Number of ticks to wait
 * Returns
 *      None
 *****************************************************/
void ece353_T32_1_wait_100mS(void);

/*****************************************************
 * Debounces Button1 using Timer32_1.
 * This function does not return until Button 1 has
 * been pressed for a minimum of 5 seconds.
 *
 * Waiting 5 Seconds will require you to call
 * ece353_T32_1_wait_100mS multiple times.
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_button1_wait_for_press(void);

/*******************************************************
 * Init hardware resources used to control S1
 * on the MKII
 ******************************************************/
void ece353_MKII_S1_Init(void);

/*******************************************************
 * Init hardware resources used to control S1
 * on the MKII
 ******************************************************/
void ece353_MKII_S2_Init(void);

/*******************************************************
 * Returns if MKII.S1 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is NOT pressed
 ******************************************************/
bool ece353_MKII_S1(void);

/*******************************************************
 * Returns if MKII.S2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is NOT pressed
 ******************************************************/
bool ece353_MKII_S2(void);

/*******************************************************
 * Sets the PWM period of the Buzzer. The duty cycle
 * will beset to 50%
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_Init(uint16_t ticks_period);

/*******************************************************
 * Turns the Buzzer on
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_On(void);

/*******************************************************
 * Turns the Buzzer off
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_Off(void);

/*******************************************************
 * Check Buzzer Run Status
 *
 * Parameters
 *      None
 * Returns
 *      True if TimerA0 is On.
 *      False if TimerA0 is Off.
 ******************************************************/
bool ece353_MKII_Buzzer_Run_Status(void);

//*****************************************************************************
//*****************************************************************************
// ICE 04 - PWMing MKII tri-color LED.
//*****************************************************************************
//*****************************************************************************

/*****************************************************
 * Initialize the 3 GPIO pins that control the RGB
 * LED on the MKII.
 *
 * Parameters
 *      en_primary_function  - If set to true, enable the primary function
 *
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_IO_Init(bool en_primary_function);

/*****************************************************
 * Sets the PWM levels for the MKII RGBLED
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *      ticks_red_on    :   Number of Ticks RED is on
 *      ticks_green_on  :   Number of Ticks GREEN is on
 *      ticks_blue_on   :   Number of Ticks BLUE is on
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_PWM(
        uint16_t ticks_period,
        uint16_t ticks_red_on,
        uint16_t ticks_green_on,
        uint16_t ticks_blue_on
);

//*****************************************************************************
//*****************************************************************************
// Example 05 - Timer32 Interrupts - Debounce S1
//*****************************************************************************
//*****************************************************************************

/*****************************************************
 * Configures Timer32_1 to generate a periodic interrupt
 *
 * Parameters
 *      ticks  - Number of milliseconds per interrupt
 * Returns
 *      None
 *****************************************************/
void ece353_T32_1_Interrupt_Ms(uint16_t ms);

/*****************************************************
 * Configures ADC14
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_ADC14_PS2_X(void);

//*****************************************************************************
//*****************************************************************************
// ICE 06 - ADC14
//*****************************************************************************

/******************************************************************************
 * Configure the IO pins for BOTH the X and Y directions of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].  The Y direction should be configured to place the results in MEM[1].
 *
 * After BOTH analog signals have finished being converted, a SINGLE interrupt
 * should be generated.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY(void);

/*****************************************************
 * Turn RGB on the MKII LED ON/Off.
 *
 * Parameters
 *  red:    if true,  turn RED LED on
 *          if false, turn RED LED off
 *  green:  if true,  turn GREEN LED on
 *          if false, turn GREEN LED off
 *  blue:   if true,  turn BLUE LED on
 *          if false, turn BLUE LED off
 *****************************************************/
void ece353_MKII_RGB_LED(bool red, bool green, bool blue);

//*****************************************************************************
//*****************************************************************************
// ICE 07 - ADC14 Comparator
//*****************************************************************************
/******************************************************************************
 * Configure the IO pins for the X direction of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].
 *
 * The ADC14 should be configured to generate interrupts using the Window
 * comparator.  The HI0 threshold should be set to 2.5V.  The LO0 threshold
 * should be set to 0.85V.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY_COMP(void);

#endif
