#include <Wire.h>

void setup() {
    Wire.begin(8);// Slave ID #8
    Wire.onReceive(recieveEvent);
    Serial.begin(74880);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("start control One LED2(control LED on slave) -- slave(ID=8).");
}

void loop() {
    delay(5000);
    Serial.println("loop.");
}

void recieveEvent() {
    while (Wire.available()) {
        byte b = Wire.read();
        Serial.print("recieved : ");
        Serial.print(b);
        Serial.println(", LED status will be changed.");
        digitalWrite(LED_BUILTIN, b);
    }
}

