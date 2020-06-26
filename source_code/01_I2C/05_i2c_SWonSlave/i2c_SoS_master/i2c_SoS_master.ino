#include <Wire.h>

byte led_status = 0;

void setup(){
    Wire.begin();
    Serial.begin(74880);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, led_status);
    Serial.println("start Switch on Slave -- master.");
}

void loop(){
    Serial.println("send request to slave(ID=8), now wait to recieve from slave.");
    Wire.requestFrom(8, 1);// request 1 bytes from Slave ID #8

    while (Wire.available()) {
        byte b = Wire.read();
        Serial.print("recieved : ");
        Serial.print(b);
        if(led_status != b){
            Serial.println(", LED status was changed.");
            led_status = b;
            digitalWrite(LED_BUILTIN, led_status);
        }else{
            Serial.println("");
        }
    }
    delay(50);
}
