/*
  Program to send 10 40kHz pulses using a timer interrupt
*/

#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT

#define EMIT_PIN 20
#define RECV_PIN 5

unsigned emit_time;
unsigned recv_time;

volatile byte emit_state;
volatile byte recv_state;

void setup() {
    // Set up clock cycle access
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    // Pin configs
    pinMode(EMIT_PIN, OUTPUT);
    pinMode(RECV_PIN, INPUT);

    // Timer1 setup--for emitter
    Timer1.initialize(12);
    Timer1.attachInterrupt(pulse_isr);
    Timer1.stop();

    Serial.begin(9600);
}

void loop() {
    pulse();
    // Serial.printf("Emit time: %lu, Recv time: %lu, Elapsed: %lu (%.3f us)\n", emit_time, recv_time, 
    //     recv_time - emit_time, (recv_time - emit_time)/120.0);
    Serial.printf("378.0 %.3f 376.0\n", (recv_time - emit_time)/120.0);
    // delayMicroseconds(2000);
    delay(10);
}

void pulse() {
    emit_state = 0;
    recv_state = 0;

    // Start timer and wait for received signal
    Timer1.start();
    while (recv_state != 1);
}

void pulse_isr() {
    // Send a pulse
    if (emit_state == 0) {
        emit_time = CYCLE;
        digitalWriteFast(EMIT_PIN, HIGH);
        emit_state = 1;
    } else {
        digitalWriteFast(EMIT_PIN, LOW);
        Timer1.stop();
        attachInterrupt(RECV_PIN, receive_isr, FALLING);
    }
}

void receive_isr() {
    recv_time = CYCLE;
    detachInterrupt(RECV_PIN);
    recv_state = 1;
}