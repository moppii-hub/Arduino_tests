#include <Wire.h>

byte b = 0;

void sendData(void);

void setup(){
    Wire.begin();
    Serial.begin(74880);
    Serial.println("start control One LED2(control LED on slave) -- master.");
}

void loop(){
    Serial.println("send data to slave(ID=8), LED on slave will be changed.");
    b = b!=0 ? 0 : 1;
    sendData();
    delay(1000);
}

void sendData(void){
    Wire.beginTransmission(8);
    Wire.write(b);
    Wire.endTransmission();
}