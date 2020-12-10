/*
 * music.c
 *
 * Author: Aksel Torgerson
 *
 */

#include "music.h"

Note_t jumpSound[] =
{
     {NOTE_C6,ONE_QUARTER,true},  // Tone, Time, Delay
     {NOTE_B5,ONE_QUARTER,true},
     {NOTE_D6,ONE_QUARTER,true},
     {NOTE_C6,ONE_QUARTER,true},
     {NOTE_F6,ONE_QUARTER,true},
     {NOTE_E6,ONE_QUARTER,true},
     {NOTE_G6,ONE_QUARTER,true},
     {NOTE_F6,ONE_QUARTER,true},
     {NOTE_A6,ONE_QUARTER,true},
     {NOTE_A6,ONE_QUARTER,true},
     {NOTE_A6,ONE_QUARTER,true},
     {NOTE_A6,ONE_QUARTER,true},
     {NOTE_A6,ONE_HALF,false},
};


//***************************************************************
// This function returns how long an individual note is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case ONE_QUARTER:
        {
            time_return  = time_return / 4;
            break;
        }
        case ONE_HALF:
        {
            time_return  = time_return / 2;
            break;
        }
        case ONE_EIGTH:
        {
            time_return  = time_return / 8;
            break;
        }
        case THREE_EIGTH:
        {
            time_return = time_return * 3;
            time_return  = time_return / 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return time_return - DELAY_AMOUNT;

}


//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_play_note(uint16_t note_index) {

    uint32_t song_delay;

    // set the pwm pulse
    set_PWM_pulse(jumpSound[note_index].period);

    // get the proper song delay
    song_delay = music_get_time_delay(jumpSound[note_index].time);

    // play the buzzer for the certain delay
    ece353_MKII_Buzzer_On();

    // delay
    ece353_T32_1_wait(song_delay);

    // turn the buzzer off
    ece353_MKII_Buzzer_Off();

    // if post-note delay is true
    if (jumpSound[note_index].delay) {

        // delay for the DELAY_AMOUNT
        ece353_T32_1_wait(DELAY_AMOUNT);
    }
}


//***************************************************************
// Plays the song (loop through, playing each note)
// and then returns
//***************************************************************
void music_play_song(void) {

    int i = 0;

    // loop through and play every note in the song
    for (i = 0; i < SONG_NUM_NOTES + 1; i++) {
        music_play_note(i);
    }

    // reinitialize the timer32_1 interrupt
    ece353_T32_1_Interrupt_Ms(10);

}

//***************************************************************
// Modify the TIMERA registers to reflect the necessary
// pulse needed to produce certain notes
//***************************************************************
void set_PWM_pulse(uint16_t pwm_period) {

    // Set the period of the timer.
    TIMER_A0->CCR[0] = pwm_period - 1;

    // Configure BUZZER Duty Cycle to 50%
    TIMER_A0->CCR[4] = (pwm_period / 2) - 1;

}
