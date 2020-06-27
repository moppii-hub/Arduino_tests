/*
For reference:
 - http://usicolog.nomaki.jp/engineering/avr/avrPWM.html
 - http://usicolog.nomaki.jp/engineering/avr/avrInterrupt.html

*/


#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#include <avr/pgmspace.h>

uint32_t timer_count[3] = {0, 0, 0};
uint16_t time_sec[3] = {0, 0, 0};
bool note_onoff[3] = {false, false, false};
byte note_num[3] = {48, 60, 72};

void setup(void){

    //start setting AVR timers ()
    cli();

    //Timer0 (set to 625Hz)
    TCCR0A = 0b00000010;        //CTC mode
    TCCR0B = 0b00000100;        //prescaler=256(100)
    OCR0A  = 100;                // (16,000,000 / 256) / 625     -> 625 times per sec
    TIMSK0 = 0b00000010;        //compare with OCR0A value

    //Timer1 (set to 1kHz)
    TCCR1B = 0b00001010;        //CTC mode, prescaler=8(010)
    TCCR1A = 0b00000000;
    OCR1A  = 2000;                // (16,000,000 / 8) / 1000     -> 1000 times per sec
    TIMSK1 = 0b00000010;        //compare with OCR1A value

    //Timer2 (set to 125Hz)     ??
    TCCR2A = 0b00000010;        //CTC mode
    TCCR2B = 0b00000101;        //prescaler=1024(101)
    OCR2A  = 125;                // (16,000,000 / 1024) / 125     -> 125 times per sec  ???
    TIMSK2 = 0b00000010;        //compare with OCR2A value

    sei();

//    Serial.begin(115200);
    MIDI.begin(0);

}

void loop(void){}

ISR(TIMER0_COMPA_vect) {            //call rate = 625Hz
    timer_count[0]++;
//    Serial.println(timer_count);
    if(timer_count[0] >= 1250){      //run at every 2.0 sec
        timer_count[0] = 0;
        time_sec[0]++;
//        Serial.print("TIMER0 : ");
//        Serial.println(time_sec[0]);
        note_onoff[0] = !note_onoff[0];
        if(note_onoff[0]) MIDI.sendNoteOn(note_num[0],127,1);
        else MIDI.sendNoteOff(note_num[0],0,1);
    }
}

ISR(TIMER1_COMPA_vect) {            //call rate = 1kHz
    timer_count[1]++;
//    Serial.println(timer_count);
    if(timer_count[1] >= 1000){      //run at every 1.0 sec
        timer_count[1] = 0;
        time_sec[1]++;
//        Serial.print("            TIMER1 : ");
//        Serial.println(time_sec[1]);
        note_onoff[1] = !note_onoff[1];
        if(note_onoff[1]) MIDI.sendNoteOn(note_num[1],127,1);
        else MIDI.sendNoteOff(note_num[1],0,1);
    }
}


ISR(TIMER2_COMPA_vect) {            //call rate = 125Hz  ??
    timer_count[2]++;
//    Serial.println(timer_count);
    if(timer_count[2] >= 6250){      //run at every 10.0 sec  ???
        timer_count[2] = 0;
        time_sec[2]++;
//        Serial.print("                        TIMER2 : ");
//        Serial.println(time_sec[2]);
        note_onoff[2] = !note_onoff[2];
        if(note_onoff[2]) MIDI.sendNoteOn(note_num[2],127,1);
        else MIDI.sendNoteOff(note_num[2],0,1);
    }
}

