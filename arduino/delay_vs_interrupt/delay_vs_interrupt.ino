#include <TimerOne.h>

#define CYCLE ARM_DWT_CYCCNT

int timer_vals[100];
int timer_count = 0;
int delay_vals[100];

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    Serial.begin(9600);
    delay(1000);

    // Start timer1
    Timer1.initialize(12);
    Timer1.attachInterrupt(timer_isr); // toggle pin every 25us
}

void loop() {
}

void timer_isr() {
    timer_vals[timer_count++] = CYCLE;

    if (timer_count == 100) {
        Timer1.detachInterrupt();
        startDelay();
    }
}

void startDelay() {
    for (int i = 0; i < 100; i++) {
        delay_vals[i] = CYCLE;
    }

    printResults();
}

void printResults() {
    Serial.printf("***Timer1***\n");
    for (int i = 0; i < 99; i++) {
        Serial.printf("Diff: %.3f\n", (timer_vals[i+1] - timer_vals[i])/96.0);
    }
    Serial.printf("***Delay***\n");
    for (int i = 0; i < 99; i++) {
        Serial.printf("Diff: %.3f\n", (delay_vals[i+1] - delay_vals[i])/96.0);
    }
}
