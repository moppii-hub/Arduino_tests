//LED TEST

int value = 0;

void setup(){
    for(int i=4;i<=11;i++){
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

void loop(){
    for(int i=6;i<=11;i++){
        delay(500);
        digitalWrite(i, value);
    }
    value = value ? 0 : 1;
}
