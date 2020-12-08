// Author: Joe Krachey
// Author: Aksel Torgerson
// Author: Matthew Kesler

#include "ece353.h"

//****************************************************
//****************************************************
// 02-Ex-Code-Organization
//****************************************************
//****************************************************

/* ***************************************************
 * Initialize hardware resources used to control LED1
 *****************************************************/
void ece353_led1_init(void) {

    // set the direction as an output
    P1->DIR |= BIT0;

    // turn off LED
    P1->OUT &= ~BIT0;

}

/*****************************************************
 * Initialize hardware resources used to control Button1
 *****************************************************/
void ece353_button1_init(void) {

    // set direction as an input, bit 1 needs to be a 0
    P1->DIR &= ~BIT1;

    // enable internal resistor
    P1->REN |= BIT1;

    // configure as a pull-up, bit 1 needs to be a 1
    P1->OUT |= BIT1;

}

/*****************************************************
 * Turn LED1 ON/Off.
 *
 * Parameters
 *  on: if true,  turn LED on
 *      if false, turn LED off
 *****************************************************/
void ece353_led1(bool on) {

    if (on) {

        // turn LED on by setting the bit
        P1->OUT |= BIT0;

    } else {

        // turn LED off by clearing the bit
        P1->OUT &= ~BIT0;

    }

}

/*****************************************************
 * Returns if Button1 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is NOT pressed
 *****************************************************/
bool ece353_button1(void) {

    // return true if pressed, else false (ACTIVE LOW)
    if ((P1->IN & BIT1) == 0) { // masking away all bits except for bit 1

        // if equal to 0, means true
        return true;

    } else {

        return false;

    }

}

//****************************************************
//****************************************************
// 02-ICE-Code-Organization
//****************************************************
//****************************************************

/*****************************************************
 * Initialize hardware resources used to control RGBLED
 *****************************************************/
void ece353_rgb_init(void) {

    // set the direction as an output for all RGB LEDs
    P2->DIR |= BIT0;
    P2->DIR |= BIT1;
    P2->DIR |= BIT2;

    // turn off LEDs
    P2->OUT &= ~BIT0;
    P2->OUT &= ~BIT1;
    P2->OUT &= ~BIT2;

}

/*****************************************************
 * Turn RGBLED ON/Off.
 *
 * Parameters
 *  red_on      :   if true,  turn RGBLED.RED on
 *                  if false, turn RGBLED.RED off
 *  green_on    :   if true,  turn RGBLED.GREEN on
 *                  if false, turn RGBLED.GREEN off
 *  blue_on     :   if true,  turn RGBLED.BLUE on
 *                  if false, turn RGBLED.BLUE off
 *****************************************************/
void ece353_rgb(bool red_on, bool green_on, bool blue_on) {

    if (red_on) {

        // turn RGBLED.RED on
        P2->OUT |= BIT0;

    } else {

        // turn RGBLED.RED off
        P2->OUT &= ~BIT0;

    }

    if (green_on) {

        // turn RGBLED.RED on
        P2->OUT |= BIT1;

    } else {

        // turn RGBLED.RED off
        P2->OUT &= ~BIT1;

    }

    if (blue_on) {

        // turn RGBLED.RED on
        P2->OUT |= BIT2;

    } else {

        // turn RGBLED.RED off
        P2->OUT &= ~BIT2;

    }

}

/*****************************************************
 * Initialize hardware resources used to control Button2
 *****************************************************/
void ece353_button2_init(void) {

    // set direction as an input, bit 4 needs to be a 0
    P1->DIR &= ~BIT4;

    // enable internal resistor
    P1->REN |= BIT4;

    // configure as a pull-up, bit 4 needs to be a 1
    P1->OUT |= BIT4;

}

/*****************************************************
 * Returns if Button2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button2 is pressed
 *      false   :   Button2 is NOT pressed
 *****************************************************/
bool ece353_button2(void) {

    // return true if pressed, else false (ACTIVE LOW)
    // masking away all bits except for bit 4
    if ((P1->IN & BIT4) == 0) {

        // if equal to 0, means true
        return true;

    } else {

        return false;

    }
}

/*******************************************************
 * Busy waits for a given number of clock cycles
 *
 * Parameters
 *      ticks   :   Number of ticks to wait
 *
 * Returns
 *      none
 ******************************************************/
void ece353_T32_1_wait(uint32_t ticks) {

        // STOP THE TIMER
        TIMER32_1->CONTROL = 0;

        // Set the timer to be a 32-bit, one-shot
        TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

        // Set the load register
        TIMER32_1->LOAD = ticks;

        // Start the timer
        TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

        // Wait until it reaches 0
        while(TIMER32_1->VALUE != 0) {
            // timer is busy waiting
        }
}



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
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_T32_1_wait_100mS(void) {

    // STOP THE TIMER
    TIMER32_1->CONTROL = 0;

    // Set the timer to be a 16-bit, one-shot
    TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT;

    // set the timer to have a prescale value of b01
    TIMER32_1->CONTROL |= TIMER32_CONTROL_PRESCALE_1;

    // Set the load register
    // 18750 = 3MHz/16 * 100ms
    TIMER32_1->LOAD = 18750;

    // Start the timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    // Wait until it reaches 0
    while(TIMER32_1->VALUE != 0) {
        // timer is busy waiting
    }

}

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
void ece353_button1_wait_for_press(void) {

    // define a counter that keeps track of how many times weve waited 100ms
    uint8_t count = 0;

    while (1) {
        // wait 100ms 50 times
        while (ece353_button1() && count < 50) {
            ece353_T32_1_wait_100mS();
            count += 1;
        }

        if (count >= 50) {
            return;
        } else {
            count = 0;
        }
    }
}

/*******************************************************
 * Init hardware resources used to control S1
 * on the MKII
 ******************************************************/
void ece353_MKII_S1_Init(void) {

    // configure as input
    P5->DIR &= ~BIT1;

    // no need for internal pullup resistor because
    // there is already one in the schematics
}

/*******************************************************
 * Init hardware resources used to control S2
 * on the MKII
 ******************************************************/
void ece353_MKII_S2_Init(void) {

    // configure as input
    P3->DIR &= ~BIT5;

    // no need for internal pullup resistor because
    // there is already one in the schematics
}

/*******************************************************
 * Returns if MKII.S1 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is NOT pressed
 ******************************************************/
bool ece353_MKII_S1(void) {

    if ((P5->IN & BIT1) == 0) {
        return true;
    } else {
        return false;
    }
}

/*******************************************************
 * Returns if MKII.S2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button2 is pressed
 *      false   :   Button2 is NOT pressed
 ******************************************************/
bool ece353_MKII_S2(void) {

    if ((P3->IN & BIT5) == 0) {
        return true;
    } else {
        return false;
    }
}

/*******************************************************
 * Sets the PWM period of the Buzzer. The duty cycle
 * will be set to 50%
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_Init(uint16_t ticks_period) {

    // Set P2.7 to be a GPIO OUTPUT Pin
    P2->DIR |= BIT7;

    // the TIMERA PWM controller will control the buzzer
    // on the MKII P2.7 <--> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turn off TA0
    TIMER_A0->CTL = 0;

    // Set the period of the timer.
    TIMER_A0->CCR[0] = ticks_period - 1;

    // Configure BUZZER Duty Cycle to 50%
    TIMER_A0->CCR[4] = (ticks_period / 2) - 1;

    // Configure TA0.4 for RESET/SET Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}

/*******************************************************
 * Turns the Buzzer on
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_On(void) {

    // clear the current Mode Control Bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set Mode Control to UP and Clear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

/*******************************************************
 * Turns the Buzzer off
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************/
void ece353_MKII_Buzzer_Off(void) {

    // turn off the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/*******************************************************
 * Check Buzzer Run Status
 *
 * Parameters
 *      None
 * Returns
 *      True if TimerA0 is On.
 *      False if TimerA0 is Off.
 ******************************************************/
bool ece353_MKII_Buzzer_Run_Status(void) {

    if((TIMER_A0->CTL & TIMER_A_CTL_MC_MASK) == TIMER_A_CTL_MC__STOP) {
        return false;
    } else {
        return true;
    }
}

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
void ece353_MKII_RGB_IO_Init(bool en_primary_function)
{

    // Complete the comments below to identify which pins
    // control which LEDs.
    //
    // Replace a and c with the correct port number
    // Replace b and d with the correct pin numbers.
    // RED      : P2.6
    // GREEN    : P2.4
    // BLUE     : P5.6

    // ADD CODE that configures the RED, GREEN, and
    // BLUE LEDs to be outputs
    P2->DIR |= BIT6; // set pin 6 on port 2 for output
    P2->DIR |= BIT4; // set for output
    P5->DIR |= BIT6; // same

    // ADD CODE that selects the Primary module function
    // for all 3 pins
    if(en_primary_function) {
        P2->SEL0 |= BIT6;
        P2->SEL1 &= ~BIT6;

        P2->SEL0 |= BIT4;
        P2->SEL1 &= ~BIT4;

        P5->SEL0 |= BIT6;
        P5->SEL1 &= ~BIT6;
    }

}

/*****************************************************
 * Sets the PWM levels for the MKII RGBLED
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *      red_num         :   RED RGB Number
 *      green_num       :   GREEN RGB Number
 *      blue_num        :   BLUE RGB Number
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_PWM(
        uint16_t ticks_period,
        uint16_t red_num,
        uint16_t green_num,
        uint16_t blue_num
)
{
    // This code converts the HTML color codes into a
    // number of clock ticks used to generate the duty cyle of
    // the PWM signal.
    uint16_t ticks_red_on = (red_num * ticks_period)/255;
    uint16_t ticks_green_on = (green_num * ticks_period)/255;
    uint16_t ticks_blue_on = (blue_num * ticks_period)/255;

    // Initialze the IO pins used to control the
    // tri-color LED.
    ece353_MKII_RGB_IO_Init(true);

    // Complete the comments below that identify which
    // TimerA outputs will control the IO pins.
    //
    // Replace w and y with the correct TimerA number
    // Replace x and z with the correct CCR number.
    // RED      <--> TA0.3
    // GREEN    <--> TA0.1
    // BLUE     <--> TA2.1

    // ADD CODE BELOW
    //
    // Turn off the timer peripherals
    TIMER_A0->CTL = 0;
    TIMER_A2->CTL = 0;

    // Set the period for both TimerA peripherals.
    TIMER_A0->CCR[0] = ticks_period - 1;
    TIMER_A2->CCR[0] = ticks_period - 1;

    // Configure RED PWM duty cycle
    TIMER_A0->CCR[3] = ticks_red_on;

    // Configure GREEN PWM duty cycle
    TIMER_A0->CCR[1] = ticks_green_on;

    // Configure BLUE PWM duty cycle
    TIMER_A2->CCR[1] = ticks_blue_on;

    // Set the OUT MODE to be mode 7 for each PWM output
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    // Turn the first TimerA peripheral
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;

    // clear the current Mode Control Bits
    //TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set Mode Control to UP and Clear the current count
    //TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    // Turn the second TimerA peripheral
    TIMER_A2->CTL |= TIMER_A_CTL_MC__UP;

    // clear the current Mode Control Bits
    //TIMER_A2->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set Mode Control to UP and Clear the current count
    //TIMER_A2->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    // set clock source for the timer
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK;
    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK;
}

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
void ece353_T32_1_Interrupt_Ms(uint16_t ms) {

    // ticks = desired period / core clock period
    // 20e-3/(1/3e6) = (3e6 * 20)/1000
    uint32_t ticks = ((SystemCoreClock * ms)/1000) - 1;

    // stop the timer
    TIMER32_1->CONTROL = 0;

    // Set the load register
    TIMER32_1->LOAD = ticks;

    // Enable the Timer32 interrupt in NVIC
    // __enable_irq();
    NVIC_EnableIRQ(T32_INT1_IRQn);
    NVIC_SetPriority(T32_INT1_IRQn, 2);

    // Start Timer32 and enable interrupt
    TIMER32_1->CONTROL =    TIMER32_CONTROL_ENABLE |    // turn timer on
                            TIMER32_CONTROL_MODE |      // periodic mode
                            TIMER32_CONTROL_SIZE |      // 32 bit timer
                            TIMER32_CONTROL_IE;         // enable interrupts

}

/*****************************************************
 * Configures Timer32_2 to generate a periodic interrupt
 *
 * Parameters
 *      ticks  - Number of milliseconds per interrupt
 * Returns
 *      None
 *****************************************************/
void ece353_T32_2_Interrupt_Ms(uint16_t ms) {

    // ticks = desired period / core clock period
    // 20e-3/(1/3e6) = (3e6 * 20)/1000
    uint32_t ticks = ((SystemCoreClock * ms)/1000) - 1;

    // stop the timer
    TIMER32_2->CONTROL = 0;

    // Set the load register
    TIMER32_2->LOAD = ticks;

    // Enable the Timer32 interrupt in NVIC
    // __enable_irq();
    NVIC_EnableIRQ(T32_INT2_IRQn);
    NVIC_SetPriority(T32_INT2_IRQn, 3);

    // Start Timer32 and enable interrupt
    TIMER32_2->CONTROL =    TIMER32_CONTROL_ENABLE |    // turn timer on
                            TIMER32_CONTROL_MODE |      // periodic mode
                            TIMER32_CONTROL_SIZE |      // 32 bit timer
                            TIMER32_CONTROL_IE;         // enable interrupts

}

/*****************************************************
 * Configures ADC14
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_ADC14_PS2_X(void) {

    // Configures P6.0 (A15) the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the ADC14 Control Registers
    // Sample time of 16 ADC cycles for the first 8 analog channels
    // Allow the ADC timer to control pulsed samples
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

    // Use sampling  timer, 12-bit conversion results
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Configure memory Control register so that we associate the correct
    // analog channel with MEM[0[
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Enable Interrupts after the conversion of MEM[0] completes
    ADC14->IER0 = ADC14_IER0_IE0;

    // Enable ADC Interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

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
void ece353_ADC14_PS2_XY(void) {

    // Configure the X direction as an analog input pin.
    // Configures P6.0 (A15) the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the Y direction as an analog input pin.
    // P4.4 A(9) as the Y direction
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // NEW -- Indicate that this is a sequence-of-channels.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02 | ADC14_CTL0_CONSEQ_1;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

    // NEW -- Make sure to indicate this is the end of a sequence.
    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    ADC14->IER0 = ADC14_IER0_IE1;
    // NEW: This is not the same as what is demonstrated in the example
    // coding video.

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;


}

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
void ece353_MKII_RGB_LED(bool red, bool green, bool blue) {

    if (red) {

            // turn RGBLED.RED on
            P2->OUT |= BIT6;

        } else {

            // turn RGBLED.RED off
            P2->OUT &= ~BIT6;

        }

        if (green) {

            // turn RGBLED.RED on
            P2->OUT |= BIT4;

        } else {

            // turn RGBLED.RED off
            P2->OUT &= ~BIT4;

        }

        if (blue) {

            // turn RGBLED.RED on
            P5->OUT |= BIT6;

        } else {

            // turn RGBLED.RED off
            P5->OUT &= ~BIT6;

        }

}

//*****************************************************************************
//*****************************************************************************
// ICE 07 - ADC14 Comparator
//*****************************************************************************
#define VOLT_0P85 1056      // 0.85 /(3.3/4096)
#define VOLT_2P50 3103      // 2.50 /(3.3/4096)

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
void ece353_ADC14_PS2_XY_COMP(void)
{
    // Configure the X direction as an analog input pin.
    // Configures P6.0 (A15) the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // Indicate that this is a sequence of samples.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02 | ADC14_CTL0_CONSEQ_1;

    // Configure CTL1 to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Set up the HI0 Window
    ADC14->HI0 = VOLT_2P50;

    // Set up the LO0 Window
    ADC14->LO0 = VOLT_0P85;

    // Associate the X direction analog signal with MEM[0], indicate the end of sequence,
    // turn on the window comparator
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;
    ADC14->MCTL[0] |= ADC14_MCTLN_EOS;
    ADC14->MCTL[0] |= ADC14_MCTLN_WINC;

    // Enable interrupts when either the HI or LO thresholds of the window
    // comparator has been exceeded.  Disable all other interrupts
    ADC14->IER1 |= ADC14_IER1_INIE;
    ADC14->IER1 &= ~ADC14_IER1_HIIE;
    ADC14->IER1 &= ~ADC14_IER1_LOIE;

    // Enable ADC Interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}
