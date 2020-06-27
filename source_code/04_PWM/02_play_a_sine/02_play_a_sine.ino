// output PWM wave to D11 pin
// Need to connect D11 <-> A speaker

uint16_t timer_count;
const uint16_t sine_freq = 440;
const uint16_t Fs = 10000;
uint16_t timer_count_limit;
uint16_t timer_sample;

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

void setup(void){

    timer_count_limit = Fs/sine_freq;

    //start setting AVR timers (disable interrupt)
    cli();

    //Timer2 for output sound wave(PWM output)
    ASSR    = 0b10011111;   //set LOW to EXCLK and AS2, set HIGH to others.
    TCCR2A  = 0b10000011;   //set HIGH to COM2A1, WGM21, and WGM20, set LOW to others.
    TCCR2B  = 0b11110001;   //set LOW to WGM22, CS22, CS21, set HIGH to others.
//    OCR2A   = 128;          //T = 16usec (128 / 16,000,000)
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

void loop(void){}


ISR(TIMER1_COMPA_vect) {            //call rate = 10kHz

//    timer_sample = 256 * (timer_count / timer_count_limit)
    timer_sample = 256*timer_count / timer_count_limit;
    OCR2A = sinetable[timer_sample];

    timer_count++;
    if(timer_count > timer_count_limit) timer_count=0;         //if (timer_count/Fs) > (1/sine_freq)
}

