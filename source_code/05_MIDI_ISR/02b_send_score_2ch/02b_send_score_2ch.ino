#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#include "macros.h"
#include "ch1_score.h"
#include "ch2_score.h"
/*
In these header include the followings variables.
    byte score_ch2[SCORE_LENGTH][2];
    byte score_ch1[SCORE_LENGTH][2];
Like these:
    byte score_ch2[SCORE_LENGTH][2] = {
                                         // ------------ INTRO 1 ------------
                      {72  , NT_ON    }, // 1 ----1
                      {36  , NT_REMAIN}, // 2
                      {79  , NT_ON    }, // 3
                      ...
                      };
*/


uint32_t timer_count;
uint16_t time_sec;
int step;
uint16_t tempo_limit;

void setup(void){
    time_sec = 0;
    time_sec = 0;
    step = 0;
    tempo_limit = 1000 / (TEMPO_BPM*STEP_LENGTH/240);

    //start setting AVR timers (disable interrupt)
    cli();

    //Timer0 (set to 625Hz)
    TCCR0A = 0b00000010;        //CTC mode
    TCCR0B = 0b00000011;        //prescaler=64(011)
    OCR0A  = 250;                // (16,000,000 / 64) / 1000     -> 1000 times per sec
    TIMSK0 = 0b00000010;        //compare with OCR0A value

    //enable interrupt
    sei();

    MIDI.begin(0);
}

void loop(void){}

ISR(TIMER0_COMPA_vect) {            //call rate = 1kHz
    timer_count++;
    if(timer_count >= tempo_limit){
        if(step < 0) return;

        timer_count = 0;
        time_sec++;

        //check channel 1
        switch(score_ch1[step][1]){
            case NT_OFF:
                MIDI.sendNoteOff(score_ch1[step][0], score_ch1[step][1], 1);
                break;
            case NT_ON:
                MIDI.sendNoteOn(score_ch1[step][0], score_ch1[step][1], 1);
                break;
            case NT_REMAIN:
                break;
        }

        //check channel 2
        switch(score_ch2[step][1]){
            case NT_OFF:
                MIDI.sendNoteOff(score_ch2[step][0], score_ch2[step][1], 2);
                break;
            case NT_ON:
                MIDI.sendNoteOn(score_ch2[step][0], score_ch2[step][1], 2);
                break;
            case NT_REMAIN:
                break;
        }

        step++;
        if(step >= SCORE_LENGTH){
            if(LOOP_FLAG) step = 0;
            else step = -1;
        }
    }
}

