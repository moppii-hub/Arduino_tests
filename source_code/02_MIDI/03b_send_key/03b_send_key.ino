//reference : 01_I2C -> 07_i2c_SWandLEDmatrix -> i2c_matrix_slave
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

int led_status[6][2];
int sw_status[6][2];
static const unsigned int notenum_table[6][2] = {
    60,66,
    61,67,
    62,68,
    63,69,
    64,70,
    65,71
};


void pinInit(){
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    for(int i=4;i<=11;i++){
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
}
void statusInit(){
    for(int i=0;i<6;i++)for(int j=0;j<2;j++){
        led_status[i][j] = 0;
        sw_status[i][j] = 0;
    }
}
void matrix_scan(){
    int out_count, in_count;
    int value;
    static int count = 0;

    out_count = count%6;
    in_count = (count < 6) ? 0 : 1;
    count=(count+1)%12;

    //1.common output
    digitalWrite(out_count+6, LOW);

    //2. LED output(anode)      but no use...
    if(in_count){
        digitalWrite(4, LOW);
        digitalWrite(5, led_status[out_count][in_count]);
    }else{
        digitalWrite(4, led_status[out_count][in_count]);
        digitalWrite(5, LOW);
    }

    delay(1);

    //3.key input
    value = digitalRead(in_count+2);
    if(value == sw_status[out_count][in_count]){   //Changed.
        sw_status[out_count][in_count] = value ? 0 : 1;
        if(!value){     //Pressed, send NOTE-ON
            MIDI.sendNoteOn(notenum_table[out_count][in_count],127,1);
        }else{//Released, send NOTE-OFF
            MIDI.sendNoteOff(notenum_table[out_count][in_count],0,1);
        }
    }

    //4. output reset
    digitalWrite(out_count+6, HIGH);
    if(in_count){
        digitalWrite(5, LOW);
    }else{
        digitalWrite(4, LOW);
    }
}

void setup(){
    pinInit();
    statusInit();
    MIDI.begin(0);
}

void loop(){
    matrix_scan();
}
