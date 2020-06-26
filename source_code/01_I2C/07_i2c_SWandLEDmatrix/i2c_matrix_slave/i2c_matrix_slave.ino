#include <Wire.h>

int led_status[6][2];
int sw_status[6][2];

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

    //2. LED output(anode)

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
        /*
        if(!value){//Pressed.
            Serial.println(out_count+(in_count*6));
        }else{//Released
        }
        */
    }

    //4. output reset
    digitalWrite(out_count+6, HIGH);
    if(in_count){
        digitalWrite(5, LOW);
    }else{
        digitalWrite(4, LOW);
    }
}

void requestEvent() {
//    Serial.println("[Slave] requestEvent");
    returnSwStatus();
}
void returnSwStatus(){
    byte upper, lower;
    lower = 0b00000001;
    upper = 0b00000010;
    for(int i=5;i>=0;i--){
        lower = lower << 1;
        upper = upper << 1;
        lower |= ((byte)sw_status[i][0]);
        upper |= ((byte)sw_status[i][1]);
    }
    Wire.write(upper);
    Wire.write(lower);
}

void recieveEvent(){
//    Serial.println("[Slave] recieveEvent");
    while (Wire.available()) {
        byte b = Wire.read();
        if(b & 0b01000000){                 //lower data
            int value = (int)(b-0b01000000);
            for(int i=0;i<6;i++){
                led_status[i][0] = value%2;
                value /= 2;
            }
        }else if(b & 0b10000000){           //upper data
            int value = (int)(b-0b10000000);
            for(int i=0;i<6;i++){
                led_status[i][1] = value%2;
                value /= 2;
            }
        }
    }
}

void setup(){
    pinInit();
    statusInit();

    Wire.begin(8);// Slave ID #8
    Wire.onRequest(requestEvent);
    Wire.onReceive(recieveEvent);

    Serial.begin(74880);
    Serial.println("start slave_matrix(I2C ID=8).");
}

void loop(){
    matrix_scan();
}
