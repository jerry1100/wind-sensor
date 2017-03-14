#define CYCLE ARM_DWT_CYCCNT
#define NOP __asm__ __volatile__ ("nop\n\t");

#define TRIG_PIN1 14
#define INT_PIN1 15
#define TRIG_PIN2 16
#define INT_PIN2 17
#define RESET_PIN 12

volatile int startCycle;
volatile int endCycle;

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Pin configurations
    pinMode(TRIG_PIN1, OUTPUT); // trigger and interrupt 1
    digitalWrite(TRIG_PIN1, HIGH);
    pinMode(INT_PIN1, INPUT);
    attachInterrupt(INT_PIN1, handler1, FALLING);
    pinMode(TRIG_PIN2, OUTPUT); // trigger and interrupt 2
    digitalWrite(TRIG_PIN2, HIGH);
    pinMode(INT_PIN2, INPUT);
    attachInterrupt(INT_PIN2, handler2, FALLING);
    pinMode(RESET_PIN, INPUT_PULLUP); // reset pin
    
    Serial.begin(9600);
}

void loop() {
    int arr[100];
    int i;
    delayMicroseconds(1000000);
    
    // Get trials
    for (i = 0; i < 100; i++) {
        delayMicroseconds(1);
    
        // Start test
        digitalWrite(TRIG_PIN1, LOW); // start counting
        delayMicroseconds(1);
        digitalWrite(TRIG_PIN2, LOW); // stop counting
        // End test

        delayMicroseconds(1);
        arr[i] = endCycle - startCycle;

        // Reset
        digitalWrite(TRIG_PIN1, HIGH);
        digitalWrite(TRIG_PIN2, HIGH);
    }

    // Print results
    int sum = 0;
    for (i = 0; i < 100; i++) {
        Serial.printf("%d\r\n", arr[i]);
        sum += arr[i];
    }
    Serial.printf("Average: %lf\r\n", (double) sum / i);
    while (digitalRead(RESET_PIN) == HIGH);
    Serial.printf("Restarting...\r\n");
}

void handler1() {
    startCycle = CYCLE;
}

void handler2() {
    endCycle = CYCLE;
}
