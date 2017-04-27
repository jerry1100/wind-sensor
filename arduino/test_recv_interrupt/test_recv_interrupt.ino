#include <TimerOne.h>

#define EMIT_PIN 5
#define RECV_PIN 20

byte emit_state = 0; // 0 if low, 1 if high

void setup() {
    pinMode(EMIT_PIN, OUTPUT);
    pinMode(RECV_PIN, INPUT);

    Timer1.initialize(12);
    Timer1.attachInterrupt(toggle); // toggle pin every 25us

    attachInterrupt(RECV_PIN, receive_isr, FALLING); // start listening
    Serial.begin(9600);
}

void loop() {
}

void toggle() {
    emit_state = !emit_state;
    digitalWrite(EMIT_PIN, emit_state);
}

void receive_isr() {
    Serial.printf("I can hear!\n");
    delay(1000);
}