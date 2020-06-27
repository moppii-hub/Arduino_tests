// output PWM wave to D11 pin
// Need to connect D11 <-> A speaker

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const uint16_t Fs = 10000;
uint16_t sine_freq[2] = {440, 440};       //can change
uint16_t timer_count[2];        //When running ISR(), will count up.
uint16_t timer_count_limit[2];  //These is a judge condition to clear timer_count.
uint16_t timer_sample[2];       //The indicator to access sinetable[].
float volume[2] = {0, 0};
float note_num_list[128];


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

void setMidiNoteNumber(void){
    note_num_list[127] = 12543.85;
    note_num_list[126] = 11839.82;
    note_num_list[125] = 11175.3;
    note_num_list[124] = 10548.08;
    note_num_list[123] = 9956.06;
    note_num_list[122] = 9397.27;
    note_num_list[121] = 8869.84;
    note_num_list[120] = 8372.02;
    note_num_list[119] = 7902.13;
    note_num_list[118] = 7458.62;
    note_num_list[117] = 7040;
    note_num_list[116] = 6644.88;
    note_num_list[115] = 6271.93;
    note_num_list[114] = 5919.91;
    note_num_list[113] = 5587.65;
    note_num_list[112] = 5274.04;
    note_num_list[111] = 4978.03;
    note_num_list[110] = 4698.64;
    note_num_list[109] = 4434.92;
    note_num_list[108] = 4186.01;
    note_num_list[107] = 3951.07;
    note_num_list[106] = 3729.31;
    note_num_list[105] = 3520;
    note_num_list[104] = 3322.44;
    note_num_list[103] = 3135.96;
    note_num_list[102] = 2959.96;
    note_num_list[101] = 2793.83;
    note_num_list[100] = 2637.02;
    note_num_list[99] = 2489.02;
    note_num_list[98] = 2349.32;
    note_num_list[97] = 2217.46;
    note_num_list[96] = 2093;
    note_num_list[95] = 1975.53;
    note_num_list[94] = 1864.66;
    note_num_list[93] = 1760;
    note_num_list[92] = 1661.22;
    note_num_list[91] = 1567.98;
    note_num_list[90] = 1479.98;
    note_num_list[89] = 1396.91;
    note_num_list[88] = 1318.51;
    note_num_list[87] = 1244.51;
    note_num_list[86] = 1174.66;
    note_num_list[85] = 1108.73;
    note_num_list[84] = 1046.5;
    note_num_list[83] = 987.77;
    note_num_list[82] = 932.33;
    note_num_list[81] = 880;
    note_num_list[80] = 830.61;
    note_num_list[79] = 783.99;
    note_num_list[78] = 739.99;
    note_num_list[77] = 698.46;
    note_num_list[76] = 659.26;
    note_num_list[75] = 622.25;
    note_num_list[74] = 587.33;
    note_num_list[73] = 554.37;
    note_num_list[72] = 523.25;
    note_num_list[71] = 493.88;
    note_num_list[70] = 466.16;
    note_num_list[69] = 440;
    note_num_list[68] = 415.3;
    note_num_list[67] = 392;
    note_num_list[66] = 369.99;
    note_num_list[65] = 349.23;
    note_num_list[64] = 329.63;
    note_num_list[63] = 311.13;
    note_num_list[62] = 293.66;
    note_num_list[61] = 277.18;
    note_num_list[60] = 261.63;
    note_num_list[59] = 246.94;
    note_num_list[58] = 233.08;
    note_num_list[57] = 220;
    note_num_list[56] = 207.65;
    note_num_list[55] = 196;
    note_num_list[54] = 185;
    note_num_list[53] = 174.61;
    note_num_list[52] = 164.81;
    note_num_list[51] = 155.56;
    note_num_list[50] = 146.83;
    note_num_list[49] = 138.59;
    note_num_list[48] = 130.81;
    note_num_list[47] = 123.47;
    note_num_list[46] = 116.54;
    note_num_list[45] = 110;
    note_num_list[44] = 103.83;
    note_num_list[43] = 98;
    note_num_list[42] = 92.5;
    note_num_list[41] = 87.31;
    note_num_list[40] = 82.41;
    note_num_list[39] = 77.78;
    note_num_list[38] = 73.42;
    note_num_list[37] = 69.3;
    note_num_list[36] = 65.41;
    note_num_list[35] = 61.74;
    note_num_list[34] = 58.27;
    note_num_list[33] = 55;
    note_num_list[32] = 51.91;
    note_num_list[31] = 49;
    note_num_list[30] = 46.25;
    note_num_list[29] = 43.65;
    note_num_list[28] = 41.2;
    note_num_list[27] = 38.89;
    note_num_list[26] = 36.71;
    note_num_list[25] = 34.65;
    note_num_list[24] = 32.7;
    note_num_list[23] = 30.87;
    note_num_list[22] = 29.14;
    note_num_list[21] = 27.5;
    note_num_list[20] = 25.96;
    note_num_list[19] = 24.5;
    note_num_list[18] = 23.12;
    note_num_list[17] = 21.83;
    note_num_list[16] = 20.6;
    note_num_list[15] = 19.45;
    note_num_list[14] = 18.35;
    note_num_list[13] = 17.32;
    note_num_list[12] = 16.35;
    note_num_list[11] = 15.43;
    note_num_list[10] = 14.57;
    note_num_list[9] = 13.75;
    note_num_list[8] = 12.98;
    note_num_list[7] = 12.25;
    note_num_list[6] = 11.56;
    note_num_list[5] = 10.91;
    note_num_list[4] = 10.3;
    note_num_list[3] = 9.72;
    note_num_list[2] = 9.18;
    note_num_list[1] = 8.66;
    note_num_list[0] = 8.18;
}


void handleNoteOn(byte channel, byte note_num, byte velocity){
    switch(channel){
        case 1:
            if(velocity){
                volume[0] = 0.5;
                sine_freq[0] = (uint16_t)note_num_list[note_num];
                timer_count[0] = 0;
                timer_count_limit[0] = Fs/sine_freq[0];
                timer_sample[0] = 0;
            }
            else volume[0] = 0;
            break;
        case 2:
            if(velocity){
                volume[1] = 0.5;
                sine_freq[1] = (uint16_t)note_num_list[note_num];
                timer_count[1] = 0;
                timer_count_limit[1] = Fs/sine_freq[1];
                timer_sample[1] = 0;
            }
            else volume[1] = 0;
            break;
        default:
            break;
    }
}

void handleNoteOff(byte channel, byte note_num, byte velocity){
    switch(channel){
        case 1:
            volume[0] = 0;
            break;
        case 2:
            volume[1] = 0;
            break;
        default:
            break;
    }
}

void setup(void){

    //Initialize the variables
    setMidiNoteNumber();

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
    int type, note, velocity, channel;
    byte r = MIDI.read();
    if (r == 1) {
        byte type = MIDI.getType();
        switch (type) {
            case 0x90:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                handleNoteOn(channel, note, velocity);
                break;
            case 0x80:
                note = MIDI.getData1();
                velocity = MIDI.getData2();
                channel = MIDI.getChannel();
                handleNoteOff(channel, note, velocity);
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
    OCR2A = (uint8_t)(sinetable[timer_sample[0]] * volume[0]) + 
            (uint8_t)(sinetable[timer_sample[1]] * volume[1]);

    //count up
    timer_count[0]++;
    if(timer_count[0] > timer_count_limit[0]) timer_count[0]=0;         //if (timer_count/Fs) > (1/sine_freq)
    timer_count[1]++;
    if(timer_count[1] > timer_count_limit[1]) timer_count[1]=0;         //if (timer_count/Fs) > (1/sine_freq)
}

