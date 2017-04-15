/*
  Program to send 10 40kHz pulses using a timer interrupt
*/

#include <TimerOne.h>

#define EMIT_PIN 5
#define NUM_PULSES 10

volatile byte count;
volatile byte state;

void setup() {
    pinMode(EMIT_PIN, OUTPUT);

    Timer1.initialize(12);
    Timer1.attachInterrupt(pulse_isr);
    Timer1.stop();
}

void loop() {
    pulse();
    delayMicroseconds(250);
}

void pulse() {
    count = 0;
    state = 0;

    Timer1.restart(); // start the timer
    while (count < NUM_PULSES*2); // wait for timer to finish
}

void pulse_isr() {
    state = !state;
    digitalWriteFast(EMIT_PIN, state);
    count++;

    if (count >= NUM_PULSES*2) {
        Timer1.stop();
    }
}