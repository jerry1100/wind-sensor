int pin = 16; // probe
int lastLow = 0;
int lastHigh = 1023;

void setup() {
    pinMode(pin, INPUT);
    Serial.begin(9600);
}

void loop() {
    int aRead = analogRead(pin);
    
    if (digitalRead(pin) == HIGH) {
        if (aRead < lastHigh) lastHigh = aRead;
        Serial.printf("HIGH, %d, Last high: %d, Last low: %d\n", aRead, lastHigh, lastLow);
    } else {
        if (aRead > lastLow) lastLow = aRead;
        Serial.printf("LOW, %d, Last high: %d, Last low: %d\n", aRead, lastHigh, lastLow);
    }
    delay(100);
}
