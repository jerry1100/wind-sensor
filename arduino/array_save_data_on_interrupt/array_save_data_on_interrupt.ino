#define CYCLE ARM_DWT_CYCCNT

// These don't change
const int mic1 = 14;
const int mic2 = 15;
const int resetPin = 12;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 2500;
const int triggers = 2*cycles;

// Variables used in interrupts
volatile int counter1;
volatile int counter2;
volatile int finished1;
volatile int finished2;
volatile int arr1[triggers];
volatile int arr2[triggers];

//#define DEBUG

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;
    
    // Pin configurations
    pinMode(mic1, INPUT_PULLUP);
    pinMode(mic2, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    attachInterrupt(mic1, handler1, FALLING);
    attachInterrupt(mic2, handler2, FALLING);
    pinMode(resetPin, INPUT_PULLUP);

    Serial.begin(9600);
}

void loop() {
    // Wait until both are ready
    // TODO do we really need this? 
    while (!finished1 || !finished2) delay(1000);
    waitForReset();
}

void handler1() {
//    Serial.printf("Entered handler1\n");
    // Save current cycle into array
    arr1[counter1++] = CYCLE;
    
    // If we're done
    if (counter1 == triggers) {
        finished1 = 1;
        detachInterrupt(mic1);
    }
//    Serial.printf("Exited handler1\n");
}

void handler2() {
//    Serial.printf("Entered handler2\n");
    // Save current cycle into array
    arr2[counter2++] = CYCLE;
    
    // If we're done
    if (counter2 == triggers) {
        finished2 = 1;
        detachInterrupt(mic2);
    }
//    Serial.printf("Exited handler2\n");
}

void waitForReset() {
    // Turn LED on to show that we're done
    digitalWrite(LED_BUILTIN, HIGH);

    // Print results
    for (int i = 0; i < triggers; i++) {
        Serial.printf("%u,%u\n", arr1[i], arr2[i]);
    }
   
    // Clear and reset
    for (int i = 0; i < triggers; i++) {
        arr1[i] = 0;
        arr2[i] = 0;
    }
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;

//    // Wait for reset then turn LED off
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);

    // Prepare interrupts
    // Could this be causing issues???
    attachInterrupt(mic1, handler1, FALLING);
    attachInterrupt(mic2, handler2, FALLING);
}

