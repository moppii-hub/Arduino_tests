//SW TEST

int val_D2 = 1, val_D3 = 1;

void setup(){
    Serial.begin(74880);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    for(int i=6;i<=11;i++) pinMode(i, OUTPUT);
    digitalWrite(6, LOW);
}

void loop(){
    int v_d2, v_d3;
    v_d2 = digitalRead(2);
    v_d3 = digitalRead(3);
    if(v_d2 == 0 && val_D2 != v_d2){
        Serial.println("SW1");
    }
    if(v_d3 == 0 && val_D3 != v_d3){
        Serial.println("SW7");
    }
    val_D2 = v_d2;
    val_D3 = v_d3;
    delay(50);
}
