#include <Wire.h>

byte b=0;

void setup() {
    Wire.begin(8);// Slave ID #8
    Wire.onRequest(requestEvent);
    Serial.begin(74880);
    Serial.println("start i2c slave(ID=8).");
}

void loop() {
    delay(5000);
    Serial.println("loop.");
}

void requestEvent() {
    Serial.println("recieved request from master, now send data to master.");
    Wire.write(b++);
}