#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
    MIDI.begin(0);
}

void loop(){
    MIDI.sendNoteOn(60,127,1);
    delay(200);

    MIDI.sendNoteOff(60,0,1);
    MIDI.sendNoteOn(62,127,1);
    delay(200);

    MIDI.sendNoteOff(62,0,1);
    MIDI.sendNoteOn(64,127,1);
    delay(350);

    MIDI.sendNoteOff(64,0,1);
    delay(50);

    MIDI.sendNoteOn(64,127,1);
    delay(200);

    MIDI.sendNoteOff(64,0,1);
    MIDI.sendNoteOn(62,127,1);
    delay(200);

    MIDI.sendNoteOff(62,0,1);
    MIDI.sendNoteOn(64,127,1);
    delay(350);

    MIDI.sendNoteOff(64,0,1);
    delay(50);

    MIDI.sendNoteOn(64,127,1);
    delay(200);

    MIDI.sendNoteOff(64,0,1);
    MIDI.sendNoteOn(62,127,1);
    delay(200);

    MIDI.sendNoteOff(64,0,1);
    delay(200*6);
}

