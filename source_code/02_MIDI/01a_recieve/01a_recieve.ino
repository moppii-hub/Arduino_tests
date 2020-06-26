#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const int tone_out_pin = 2;
byte r, type, note_num, velocity;

void setup(){
    MIDI.begin(0);
    pinMode(tone_out_pin, OUTPUT);
}

void loop(){
    r = MIDI.read();
    if(r == 1){
        type = MIDI.getType();
        switch(type) {
            case 0x90:  //NoteOn(enum)
                note_num = MIDI.getData1();
                velocity = MIDI.getData2();
                if(note_num == 60){
                    if(velocity == 127) tone(tone_out_pin, 440);
                    if(velocity == 0) noTone(tone_out_pin);
                }
                break;
            case 0x80:  //NoteOff(enum)
                noTone(tone_out_pin);
                break;
            default:
                break;
        }
    }
}
