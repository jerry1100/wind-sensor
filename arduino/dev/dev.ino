/*
    Main development program for the Ultrasonic Wind Sensor project.
 */

#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT
#define RESET_PIN 12
#define EMIT_PIN 20
#define RECV_PIN 5
#define NUM_PULSES 4

volatile byte emit_state;
volatile byte emit_count;
volatile byte recv_count;
unsigned long emit_times[NUM_PULSES];
unsigned long recv_times[NUM_PULSES];

unsigned long total_diff = 0;
unsigned long filter_diff = 0;
int total_samples = 0;
int filter_samples = 0;

float prev_result;
int count = 0;

void setup() {
    // Set up clock cycle access
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    // Pin configs
    pinMode(RESET_PIN, INPUT_PULLUP);
    pinMode(EMIT_PIN, OUTPUT);
    pinMode(RECV_PIN, INPUT);

    // Timer1 setup--for emitter
    Timer1.initialize(12); // toggle every 12us
    Timer1.attachInterrupt(pulse_isr);
    Timer1.stop();

    Serial.begin(9600);
}

void loop() {
    // Send pulse
    pulse();

    unsigned long pdiff = recv_times[NUM_PULSES-1] - emit_times[NUM_PULSES-1];
    for (int a = 1; a < 10; a++) {
        if (count == 0) {
            diff[a] = prev_diff[a] = pdiff;
        }
        float alpha = a/100.0;
        diff[a] = alpha*pdiff + (1-alpha)*prev_diff[a];
        prev_diff[a] = diff[a];
        Serial.printf("%.3f,", diff[a]/120.0);
    }
    Serial.printf("\n");
    // unsigned long diff = recv_times[0] - emit_times[0];
    
    // // Cumulate total difference
    // total_samples++;
    // total_diff += diff;

    // // Cumulate filtered difference
    // if (abs(diff - prev_result) < 60) {
    //     filter_diff += diff;
    //     filter_samples++;
    // }

    // // Wait for next run
    // if (total_samples == 500) {
    //     prev_result = total_diff/total_samples;
    //     Serial.printf("(Total) %d samples: %.3f us\n", total_samples, total_diff/120.0/total_samples);
    //     Serial.printf("(Filtr) %d samples: %.3f us\n", filter_samples, filter_diff/120.0/filter_samples);
    //     Serial.printf("\n");
    //     // Serial.printf("%.4f\n", filter_diff/120.0/filter_samples);
    //     // Serial.printf("%.4f\n", total_diff/120.0/total_samples);
    //     // while (digitalRead(RESET_PIN) == HIGH);

    //     // Reset
    //     total_diff = 0;
    //     filter_diff = 0;
    //     filter_samples = 0;
    //     total_samples = 0;

        if (count++ == 10000) {
            Serial.printf("Done");
            exit(0);
        }
    // }

    delayMicroseconds(2000); // wait for receiver to die down
}

void pulse() {
    emit_state = 0;
    emit_count = 0;
    recv_count = 0;

    Timer1.start(); // start the timer, should take 240us
    while (recv_count < NUM_PULSES); // wait until received all pulses
}

void pulse_isr() {
    emit_state = !emit_state;
    digitalWriteFast(EMIT_PIN, emit_state);
    if (emit_count % 2) {
        emit_times[emit_count/2] = CYCLE;
    }
    emit_count++;

    if (emit_count == 2*NUM_PULSES) {
        attachInterrupt(RECV_PIN, receive_isr, FALLING); // start listening
        Timer1.stop();
    } else if (emit_count > 2*NUM_PULSES) { // this shouldn't happen
        Serial.printf("emit_count greater than expected\n");
    }
}

void receive_isr() {
    // TODO add time checking code
    recv_times[recv_count] = CYCLE;
    recv_count++;

    if (recv_count == NUM_PULSES) {
        detachInterrupt(RECV_PIN);
    } else if (recv_count > NUM_PULSES) { // this shouldn't happen
        Serial.printf("recv_count greater than expected\n");
    }
}