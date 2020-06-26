//TONE TEST

const int tone_out_pin = 2;

void setup(){
    pinMode(tone_out_pin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
    tone(tone_out_pin, 440);
    delay(1000);
    noTone(tone_out_pin);
    delay(1000);
}
