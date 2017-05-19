#include <TimerOne.h>

#define EMIT_PIN 20

byte emit_state = 0; // 0 if low, 1 if high

void setup() {
    pinMode(EMIT_PIN, OUTPUT);

    Timer1.initialize(12);
    Timer1.attachInterrupt(toggle); // toggle pin every 25us
}

void loop() {
}

void toggle() {
    emit_state = !emit_state;
    digitalWriteFast(EMIT_PIN, emit_state);
}
