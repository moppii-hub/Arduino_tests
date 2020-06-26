#include <Wire.h>

static const int input_pin = 8;
byte input_value = 0;

void setup() {
    Wire.begin(8);// Slave ID #8
    Wire.onRequest(requestEvent);
    Serial.begin(74880);
    pinMode(input_pin, INPUT_PULLUP);
    Serial.println("start control Tone on Slave(based on SoS) -- slave(ID=8).");
}

void loop() {
    delay(5000);
    Serial.println("loop.");
}

void requestEvent() {
    Serial.println("recieved request from master, now send data to master.");
    input_value = digitalRead(input_pin);
    input_value = input_value ? 0 : 1;
    Wire.write(input_value);
}

