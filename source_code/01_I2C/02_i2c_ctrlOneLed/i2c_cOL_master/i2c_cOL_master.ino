#include <Wire.h>

void setup(){
    Wire.begin();
    Serial.begin(74880);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("start control One LED -- master.");
}

void loop(){
    Serial.println("send request to slave(ID=8), now wait to recieve from slave.");
    Wire.requestFrom(8, 1);// request 1 bytes from Slave ID #8

    while (Wire.available()) {
        byte b = Wire.read();
        Serial.print("recieved : ");
        Serial.print(b);
        Serial.println(", LED status will be changed.");
        digitalWrite(LED_BUILTIN, b);
    }
    delay(1000);
}
