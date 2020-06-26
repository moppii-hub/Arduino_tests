#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const int tone_out_pin = 2;
float note_num_list[127];

void setMidiNoteNumber(void){
    note_num_list[69] = 440.0;
    for (int i=68; i>=0; i--){
        note_num_list[i] = note_num_list[i+1] / (1.059463);
    }
    for (int i=70; i<127; i++){
        note_num_list[i] = note_num_list[i-1] * (1.059463);
    }
}

void handleNoteOn(byte channel, byte note_num, byte velocity){
    if(velocity == 0) noTone(tone_out_pin);
    else tone(tone_out_pin, (int)note_num_list[note_num]);
}

void handleNoteOff(byte channel, byte note_num, byte velocity){
    noTone(tone_out_pin);
}

void setup(){
    setMidiNoteNumber();
    MIDI.setHandleNoteOn(handleNoteOn);     //set callback at recieve NOTE-ON
    MIDI.setHandleNoteOff(handleNoteOff);   //set callback at recieve NOTE-OFF
    MIDI.begin(0);  //all channel
    pinMode(tone_out_pin, OUTPUT);
}

void loop(){
    MIDI.read();
}