/*
    Main development program for the Ultrasonic Wind Sensor project.
 */

#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT
#define EMIT_PIN 5
#define NUM_PULSES 10

volatile byte count;
volatile byte state;
unsigned long send_times[2*NUM_PULSES];

void setup() {
    // Set up clock cycle access
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    pinMode(EMIT_PIN, OUTPUT);

    // Timer1 setup
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

    Timer1.start(); // start the timer, should take 240us
    delayMicroseconds(240);
}

void pulse_isr() {
    state = !state;
    digitalWriteFast(EMIT_PIN, state);
    send_times[count] = CYCLE;
    count++;

    if (count == 2*NUM_PULSES) {
        Timer1.stop();
    }
}