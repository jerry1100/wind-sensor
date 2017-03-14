#define CYCLE ARM_DWT_CYCCNT

// These don't change
const int mic1 = 14;
const int mic2 = 15;
const int resetPin = 12;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 10;
const int triggers = 2*cycles;

// Counters and storage
int read1;
int read2;
unsigned cycle1;
unsigned cycle2;
int counter1;
int counter2;
int finished1;
int finished2;
unsigned arr1[triggers];
unsigned arr2[triggers];

//#define DEBUG

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Pin configurations
    pinMode(mic1, INPUT);
    pinMode(mic2, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(resetPin, INPUT_PULLUP);

    counter1 = counter2 = 0;
    
    Serial.begin(9600);
}

void loop() {
    read1 = analogRead(mic1);
    cycle1 = CYCLE;
    read2 = analogRead(mic2);
    cycle2 = CYCLE;

    // Save times
    if (!finished1 && (read1 > 771 || read1 < 280)) {
        arr1[counter1++] = cycle1;
        finished1 = (counter1 == triggers);
    }
    if (!finished2 && (read2 > 574 || read2 < 450)) {
        arr2[counter2++] = cycle2;
        finished2 = (counter2 == triggers);
    }

    // Both done, wait for reset
    if (finished1 && finished2) {
        waitForReset();
    }
}

void waitForReset() {
    #ifdef DEBUG
    Serial.printf("Waiting for reset\n");
    #endif

    // Turn LED on to show that we're done
    delayMicroseconds(100000);
    digitalWrite(LED_BUILTIN, HIGH);

    // Print results
    unsigned sum;
    int i;
    for (i = 0; i < triggers; i++) {
        Serial.printf("%u %u %u\n", arr1[i], arr2[i], arr2[i] - arr1[i]);
        sum += (arr2[i] - arr1[i]);
    }
    Serial.printf("Average of %d samples: %.3f\n", triggers, (float) sum/triggers);

    // Clear and reset
    for (i = 0; i < triggers; i++) {
        arr1[i] = arr2[i] = 0;
    }
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;

    // Wait for reset then turn LED off
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif
}

