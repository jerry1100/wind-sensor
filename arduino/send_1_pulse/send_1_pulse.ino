const int pin = 14; // emitter pin

void setup() {
    pinMode(pin, OUTPUT);
}

void loop() {
    send_1_pulse();
    delay(2);
}

void send_1_pulse() {
    digitalWrite(pin, HIGH);
    delayMicroseconds(12);
    digitalWrite(pin, LOW);
}
