#define CYCLE ARM_DWT_CYCCNT
#define NOP __asm__ __volatile__ ("nop\n\t");

#define RESET_PIN 12

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    pinMode(RESET_PIN, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    int startCycle;
    int endCycle;
    int arr[100];
    int i;
    delayMicroseconds(1000000);
    
    // Get trials
    for (i = 0; i < 100; i++) {
        delayMicroseconds(1);
        startCycle = CYCLE;
    
        // Start test
        // End test

        endCycle = CYCLE;
        delayMicroseconds(1);
        arr[i] = endCycle - startCycle - 1;
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

