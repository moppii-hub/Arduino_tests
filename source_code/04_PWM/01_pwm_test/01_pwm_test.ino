// output PWM wave to D11 pin

void setup(void){

    //start setting AVR timers (disable interrupt)
    cli();

    //Timer2 for PWM output
    ASSR    = 0b10011111;   //set LOW to EXCLK and AS2, set HIGH to others.
    TCCR2A  = 0b10000011;   //set HIGH to COM2A1, WGM21, and WGM20, set LOW to others.
    TCCR2B  = 0b11110001;   //set LOW to WGM22, CS22, CS21, set HIGH to others.
    OCR2A   = 128;          //T = 16usec (128 / 16,000,000)
    DDRB = 0b11111111;  //set output portB

    //enable interrupt
    sei();
}

void loop(void){}
