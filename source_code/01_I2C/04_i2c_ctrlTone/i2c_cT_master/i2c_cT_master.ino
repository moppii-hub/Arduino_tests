#include <Wire.h>

byte led_status = 0;
static const int reset_out_pin = 3;
static const int tone_out_pin = 3;

void setup(){
    Wire.begin();
    Serial.begin(74880);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(reset_out_pin, OUTPUT);
    pinMode(tone_out_pin, OUTPUT);
    Serial.println("start control Tone(based on i2c_SnR) -- master.");

    Serial.println("initialize reset pin(output LOW), wait 2 sec.");
    digitalWrite(LED_BUILTIN, led_status);

    delay(2000);

    Serial.println("output HIGH to reset pin.");
    digitalWrite(reset_out_pin, HIGH);
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
            if(led_status) tone(tone_out_pin, 440);
            else noTone(tone_out_pin);
        }else{
            Serial.println("");
        }
    }
    delay(50);
}
