/*
    Main development program for the Ultrasonic Wind Sensor project.
 */

#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT
#define EMIT_PIN 5
#define NUM_PULSES 10

volatile byte count;
volatile byte state;

void setup() {
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
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