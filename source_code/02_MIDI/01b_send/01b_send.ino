#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
    MIDI.begin(0);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
    delay(1000);
    MIDI.sendNoteOn(60,127,1);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    MIDI.sendNoteOff(60,0,1);
    digitalWrite(LED_BUILTIN, LOW);
}

