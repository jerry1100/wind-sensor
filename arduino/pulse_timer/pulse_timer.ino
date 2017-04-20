/*
  Program to send 10 40kHz pulses using a timer interrupt
*/

#include <TimerOne.h>

#define EMIT_PIN 5
#define RECV_PIN 20
#define NUM_PULSES 10

volatile byte emit_state;
volatile byte emit_count;
volatile byte recv_count;

void setup() {
    pinMode(EMIT_PIN, OUTPUT);
    pinMode(RECV_PIN, INPUT);

    Timer1.initialize(12);
    Timer1.attachInterrupt(pulse_isr);
    Timer1.stop();
}

void loop() {
    pulse();
    delayMicroseconds(2000);
}

void pulse() {
    emit_state = 0;
    emit_count = 0;
    recv_count = 0;
    attachInterrupt(RECV_PIN, receive_isr, CHANGE); // start listening

    Timer1.start(); // start the timer, should take 240us
    while (emit_count < 2*NUM_PULSES); // wait until done transmitting
}

void pulse_isr() {
    emit_state = !emit_state;
    emit_times[emit_count] = CYCLE;
    emit_count++;

    if (emit_count == 2*NUM_PULSES) {
        Timer1.stop();
    } else if (emit_count > 2*NUM_PULSES) { // this shouldn't happen
        Serial.printf("emit_count greater than expected\n");
    }
}

void receive_isr() {
    recv_count++;

    if (recv_count == 2*NUM_PULSES) {
        detachInterrupt(RECV_PIN);
    } else if (recv_count > 2*NUM_PULSES) { // this shouldn't happen
        Serial.printf("recv_count greater than expected\n");
    }
}