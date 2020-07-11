/*
For reference:
 - http://usicolog.nomaki.jp/engineering/avr/avrPWM.html
 - http://usicolog.nomaki.jp/engineering/avr/avrInterrupt.html
*/

// Standard Arduino Pins
#define digitalPinToPortReg(P) \
        (((P) >= 0 && (P) <= 7) ? &PORTD : (((P) >= 8 && (P) <= 13) ? &PORTB : &PORTC))
#define digitalPinToPINReg(P) \
        (((P) >= 0 && (P) <= 7) ? &PIND : (((P) >= 8 && (P) <= 13) ? &PINB : &PINC))
#define digitalPinToBit(P) \
        (((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) -8 : (P) -14))
#define digitalReadFast(P) bitRead(*digitalPinToPINReg(P), digitalPinToBit(P))
#define digitalWriteFast(P, V) bitWrite(*digitalPinToPortReg(P), digitalPinToBit(P), (V))

//uint32_t timer_count = 0;
uint8_t matrix_scan_count = 0;
//uint8_t led_status[24] = { 1,1, 0,0, 0,1, 0,0, 1,1, 0,0,
//                           1,0, 0,0, 1,1, 0,0, 1,0, 0,0};
uint8_t led_status[48] = { 1,1,1,1, 0,0,0,0, 1,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0,
                           1,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0, 1,0,0,0, 0,0,0,0};

void initGPIOpins(void){
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);

    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
}

void clearLED(void){
    //Anode
    digitalWriteFast(4, 0);
    digitalWriteFast(5, 0);

    //Cathode
    digitalWriteFast(6, 1);
    digitalWriteFast(7, 1);
    digitalWriteFast(8, 1);
    digitalWriteFast(9, 1);
    digitalWriteFast(10, 1);
    digitalWriteFast(11, 1);
}

void lightLED(uint8_t anode_pin, uint8_t cathode_pin){
    digitalWriteFast(anode_pin, 1);
    digitalWriteFast(cathode_pin, 0);
}

void setup(void){

    initGPIOpins();

    //start setting AVR timers (disable interrupt)
    cli();

/*
    //Timer0 (set to 625Hz)
    TCCR0A = 0b00000010;        //CTC mode
    TCCR0B = 0b00000100;        //prescaler=256(100)
    OCR0A  = 100;                // (16,000,000 / 256) / 625     -> 625 times per sec
    TIMSK0 = 0b00000010;        //compare with OCR0A value
*/
    //Timer1 (set to 10kHz)
    TCCR1B = 0b00001001;        //CTC mode, prescaler=1(001)
    TCCR1A = 0b00000000;
    OCR1A  = 1600;              // (16,000,000 / 1) / 1600     -> 10,000 times per sec
    TIMSK1 = 0b00000010;        //compare with OCR1A value

    //enable interrupt
    sei();

    Serial.begin(115200);
}

void loop(void){}

/*
ISR(TIMER0_COMPA_vect) {
}
*/

ISR(TIMER1_COMPA_vect) {

    if(matrix_scan_count%2 == 1) clearLED();
    else{
        if(led_status[matrix_scan_count/2]){
            if((matrix_scan_count/8)<6){
                lightLED(4, 6+(matrix_scan_count/8)%6);
            }else{
                lightLED(5, 6+(matrix_scan_count/8)%6);
            }
        }
    }
    matrix_scan_count++;
    if(matrix_scan_count>96) matrix_scan_count = 0;

}



