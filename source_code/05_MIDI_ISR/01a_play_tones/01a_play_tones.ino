// output PWM wave to D11 pin
// Need to connect D11 <-> A speaker

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const uint16_t sine_freq[2] = {261, 329};       //C, E
const uint16_t Fs = 10000;
uint16_t timer_count[2];        //When running ISR(), will count up.
uint16_t timer_count_limit[2];  //These is a judge condition to clear timer_count.
uint16_t timer_sample[2];       //The indicator to access sinetable[].
float volume[2] = {0, 0};

const uint8_t sinetable[256] PROGMEM = {
        127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,
        181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,
        223,225,227,229,231,233,234,236,238,239,240,
        242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,
        254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,
        243,242,240,239,238,236,234,233,231,229,227,225,223,
        221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,
        176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,
        124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
        76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,
        20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,
        2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
        33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,
        96,99,102,105,108,111,115,118,121,124
};


void handleNoteOn(byte channel, byte note_num, byte velocity){
    switch(note_num){
        case 60:
            if(velocity) volume[0] = 0.5;
            else volume[0] = 0;
            break;
        case 64:
            if(velocity) volume[1] = 0.5;
            else volume[1] = 0;
            break;
        default:
            break;
    }
}

void handleNoteOff(byte channel, byte note_num, byte velocity){
    switch(note_num){
        case 60:
            volume[0] = 0;
            break;
        case 64:
            volume[1] = 0;
            break;
        default:
            break;
    }
}

void setup(void){

    //Initialize the variables
    timer_count[0] = 0;
    timer_count[1] = 0;

    timer_count_limit[0] = Fs/sine_freq[0];
    timer_count_limit[1] = Fs/sine_freq[1];

    timer_sample[0] = 0;
    timer_sample[1] = 0;

    //start setting AVR timers (disable interrupt)
    cli();

    // Set baud rate to 31,250. Requires modification if clock speed is not 16MHz.
    UBRR0H = ((16000000 / 16 + 31250 / 2) / 31250 - 1) >> 8;
    UBRR0L = ((16000000 / 16 + 31250 / 2) / 31250 - 1);
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    // enable rx
    UCSR0B |= _BV(RXEN0);
    // USART RX interrupt enable bit on
    UCSR0B |= _BV(RXCIE0);

    MIDI.begin(0);  //all channel
//    MIDI.setHandleNoteOn(handleNoteOn);     //set callback at recieve NOTE-ON
//    MIDI.setHandleNoteOff(handleNoteOff);   //set callback at recieve NOTE-OFF

    //Timer2 for output sound wave(PWM output)
    ASSR    = 0b10011111;   //set LOW to EXCLK and AS2, set HIGH to others.
    TCCR2A  = 0b10000011;   //set HIGH to COM2A1, WGM21, and WGM20, set LOW to others.
    TCCR2B  = 0b11110001;   //set LOW to WGM22, CS22, CS21, set HIGH to others.
    OCR2A   = 128;        //Wave amplitude(0-255, center = 128)
    DDRB = 0b11111111;  //set output portB

    //Timer1 for calculation output signal(set to 10kHz)
    TCCR1B = 0b00001010;        //CTC mode, prescaler=8(010)
    TCCR1A = 0b00000000;
    TIMSK1 = 0b00000010;        //compare with OCR1A value
    OCR1A  = 200;                // (16,000,000 / 8) / 10000     -> 10000 times per sec

    //enable interrupt
    sei();
}

void loop(void){
    int type, note, velocity, channel, d1, d2;
    byte r = MIDI.read();
    if (r == 1) {
        byte type = MIDI.getType();
        switch (type) {
            case 0x90:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                handleNoteOn(1, note, velocity);
                break;
            case 0x80:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                handleNoteOff(1, note, velocity);
                break;
            default:
                break;
        }
    }
}

ISR(TIMER1_COMPA_vect) {            //call rate = 10kHz

    //refresh timer_sample[]
    timer_sample[0] = 256*timer_count[0] / timer_count_limit[0];
    timer_sample[1] = 256*timer_count[1] / timer_count_limit[1];

    //calc wave amplitude
    OCR2A = (sinetable[timer_sample[0]] * volume[0]) + 
            (sinetable[timer_sample[1]] * volume[1]);

    //count up
    timer_count[0]++;
    if(timer_count[0] > timer_count_limit[0]) timer_count[0]=0;         //if (timer_count/Fs) > (1/sine_freq)
    timer_count[1]++;
    if(timer_count[1] > timer_count_limit[1]) timer_count[1]=0;         //if (timer_count/Fs) > (1/sine_freq)
}

