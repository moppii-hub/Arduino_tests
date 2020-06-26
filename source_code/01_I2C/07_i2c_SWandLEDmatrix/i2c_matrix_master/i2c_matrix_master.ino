#include <Wire.h>

int led_status[6][2];
int sw_status[6][2];
int sw_status_before[6][2];
int swChangeCnt;
bool isPlaying;

static const unsigned int freq_table[6][2] = {
    262,370,
    277,392,
    294,415,
    311,440,
    330,466,
    349,494
};
static const int reset_out_pin = 3;
static const int tone_out_pin = 2;
static const int I2C_slave_num = 8;

void pinInit(){
    pinMode(reset_out_pin, OUTPUT);
    pinMode(tone_out_pin, OUTPUT);
}
void statusInit(){
    swChangeCnt = 0;
    isPlaying = false;
    for(int j=0;j<2;j++){
        for(int i=0;i<6;i++){
            led_status[i][j] = 0;
            sw_status[i][j] = 0;
            sw_status_before[i][j] = 0;
        }
    }
}
void resetSlave(){
    digitalWrite(reset_out_pin, LOW);
    delay(10);
    digitalWrite(reset_out_pin, HIGH);
}

void requestSwStatus(){
//    Serial.println("[Master] requestSwStatus");
    Wire.requestFrom(I2C_slave_num, 2);// request 2 bytes from Slave ID #8
    while (Wire.available()) {
        byte b = Wire.read();
        if(b & 0b01000000){                 //lower data
            int value = (int)(b-0b01000000);
            for(int i=0;i<6;i++){
                sw_status[i][0] = value%2;
                if(sw_status[i][0] != sw_status_before[i][0]) swChangeCnt++;
                value /= 2;
            }
        }else if(b & 0b10000000){           //upper data
            int value = (int)(b-0b10000000);
            for(int i=0;i<6;i++){
                sw_status[i][1] = value%2;
                if(sw_status[i][1] != sw_status_before[i][1]) swChangeCnt++;
                value /= 2;
            }
        }
    }
}
void sendLedStatus(){
    byte upper, lower;

//    Serial.println("[Master] sendLedStatus");

    lower = 0b00000001;
    upper = 0b00000010;
    for(int i=5;i>=0;i--){
        lower = lower << 1;
        upper = upper << 1;
        lower |= ((byte)led_status[i][0]);
        upper |= ((byte)led_status[i][1]);
    }

    Wire.beginTransmission(I2C_slave_num);

    Wire.write(upper);
    Wire.write(lower);

    Wire.endTransmission();
}

void refreshSwStatus(){
    for(int j=0;j<2;j++) for(int i=0;i<6;i++){
        if(sw_status[i][j] == sw_status_before[i][j]) continue;

        if(sw_status[i][j]){        //Pressed.
            if(isPlaying) noTone(tone_out_pin);
            tone(tone_out_pin, freq_table[i][j]);
            isPlaying = true;
        }else{                      //Released.
            noTone(tone_out_pin);
            isPlaying = false;
        }

        sw_status_before[i][j] = sw_status[i][j];
    }
}

void refreshLedStatus(){
    for(int j=0;j<2;j++) for(int i=0;i<6;i++)
        led_status[i][j] = sw_status[i][j];
}

void debug_PrintLedStatus(){
    for(int j=0;j<2;j++){
        for(int i=0;i<6;i++) Serial.print(led_status[i][j]);
        Serial.print(" ");
    }
    Serial.println(" ");
}

void setup(){
    pinInit();
    statusInit();

    resetSlave();

    Wire.begin();

    Serial.begin(74880);
    Serial.println("start master_matrix.");
}

void loop(){
    delay(10);

    requestSwStatus();
    while(swChangeCnt){
        refreshSwStatus();
        refreshLedStatus();
        sendLedStatus();
        swChangeCnt--;
    }

//    debug_PrintLedStatus();
}


