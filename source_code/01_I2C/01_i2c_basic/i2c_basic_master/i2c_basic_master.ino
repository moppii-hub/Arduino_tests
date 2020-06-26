#include <Wire.h>

void setup(){
    Wire.begin();
    Serial.begin(74880);
    Serial.println("start i2c master.");
}

void loop(){
    Serial.println("send request to slave(ID=8), now wait to recieve from slave.");
    Wire.requestFrom(8, 1);// request 1 bytes from Slave ID #8

    while (Wire.available()) {
        byte b = Wire.read();
        Serial.println(b);
    }

    Serial.println("");
    delay(500);
}
