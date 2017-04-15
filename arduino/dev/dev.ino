/*
    Main development program for the Ultrasonic Wind Sensor project.
 */

#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT
#define EMIT_PIN 5
#define RECV_PIN 20
#define NUM_PULSES 10

volatile byte emit_state;
volatile byte emit_count;
volatile byte recv_count;
unsigned long emit_times[2*NUM_PULSES];
unsigned long recv_times[2*NUM_PULSES];

void setup() {
    // Set up clock cycle access
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    // Pin configs
    pinMode(EMIT_PIN, OUTPUT);
    pinMode(RECV_PIN, INPUT_PULLUP);

    // Timer1 setup--for emitter
    Timer1.initialize(12);
    Timer1.attachInterrupt(pulse_isr);
    Timer1.stop();

    Serial.begin(9600);
}

void loop() {
    // Send pulse
    pulse();

    // Display results
    for (int i = 0; i < 2*NUM_PULSES; i++) {
        unsigned long diff = recv_times[i] - emit_times[i];
        Serial.printf("[%d] Send: %lf, Recv: %lf, Diff: %lf (%.3f us)\n",
                      i, emit_times[i], recv_times[i], diff, diff / 96.0);
    }
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
    digitalWriteFast(EMIT_PIN, emit_state);
    emit_times[emit_count] = CYCLE;
    emit_count++;

    if (emit_count == 2*NUM_PULSES) {
        Timer1.stop();
    } else if (emit_count > 2*NUM_PULSES) { // this shouldn't happen
        Serial.printf("emit_count greater than expected\n");
    }
}

void receive_isr() {
    // TODO add time checking code
    recv_times[recv_count] = CYCLE;

    if (recv_count == 2*NUM_PULSES) {
        detachInterrupt(RECV_PIN);
    } else if (recv_count > 2*NUM_PULSES) { // this shouldn't happen
        Serial.printf("recv_count greater than expected\n");
    }
}