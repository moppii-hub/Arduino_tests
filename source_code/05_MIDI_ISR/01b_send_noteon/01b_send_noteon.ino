#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

uint32_t timer_count;
uint16_t time_sec;
byte score[8][2] = {
                      {60, 127},
                      {60, 0},
                      {64, 127},
                      {64, 0},
                      {60, 127},
                      {64, 127},
                      {64, 0},
                      {60, 0}
};
byte step;

void setup(void){
    time_sec = 0;
    time_sec = 0;
    step = 0;

    //start setting AVR timers (disable interrupt)
    cli();

    //Timer0 (set to 625Hz)
    TCCR0A = 0b00000010;        //CTC mode
    TCCR0B = 0b00000100;        //prescaler=256(100)
    OCR0A  = 100;                // (16,000,000 / 256) / 625     -> 625 times per sec
    TIMSK0 = 0b00000010;        //compare with OCR0A value

    //enable interrupt
    sei();

    MIDI.begin(0);
}

void loop(void){}

ISR(TIMER0_COMPA_vect) {            //call rate = 625Hz
    timer_count++;
    if(timer_count >= 625){      //run at every 1.0 sec
        timer_count = 0;
        time_sec++;
        if(score[step][1]) MIDI.sendNoteOn(score[step][0],score[step][1],1);
        else MIDI.sendNoteOff(score[step][0],score[step][1],1);
        step = (step+1) % 8;
    }
}

