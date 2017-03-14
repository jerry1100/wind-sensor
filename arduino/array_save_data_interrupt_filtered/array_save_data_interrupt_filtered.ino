#define CYCLE ARM_DWT_CYCCNT

// Experimental values
const int prop = (int) 9.4*96000000/100/(331.4 + 0.6*25);
const int t_lower = prop*0.5;
const int t_upper = prop*1.5;

// These don't change
const int mic1 = 14;
const int mic2 = 15;
const int resetPin = 12;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 500;
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
#ifdef DEBUG
    Serial.printf("Entered handler1\n");
#endif
    
    // Save current cycle into array
    arr1[counter1++] = CYCLE;
    
    // If we're done
    if (counter1 == triggers) {
        finished1 = 1;
        detachInterrupt(mic1);
    }

#ifdef DEBUG
    Serial.printf("Exited handler1\n");
#endif
}

void handler2() {
#ifdef DEBUG
    Serial.printf("Entered handler2\n");
#endif

    // Save current cycle into array
    arr2[counter2++] = CYCLE;
    
    // If we're done
    if (counter2 == triggers) {
        finished2 = 1;
        detachInterrupt(mic2);
    }
    
#ifdef DEBUG
    Serial.printf("Exited handler2\n");
#endif
}

void waitForReset() {
//    digitalWrite(LED_BUILTIN, HIGH);

#ifdef DEBUG
    for (int i = 0; i < triggers; i++) {
        Serial.printf("%u,%u\n", arr1[i], arr2[i]);
    }
#endif

    // Process results
    int num_samples = 0;
    int sum_diff = 0;
    int i1 = 0;
    int i2 = 0;
    while (i1 < triggers && i2 < triggers) {
        int diff = arr2[i2] - arr1[i1];

        if (diff < t_lower) {
            i2++;
        } else if (diff > t_upper) {
            i1++;
        } else {
            sum_diff += diff;
            num_samples++;
            i1++;
            i2++;
        }
    }

    // Print results
    int average = sum_diff / num_samples;
    Serial.printf("%d sample results (out of %d) | Theoretical: %d %dus | Experimental: %d %dus\n",
        num_samples, triggers, prop, prop/96, average, average/96); 
   
    // Clear and reset
    for (int i = 0; i < triggers; i++) {
        arr1[i] = 0;
        arr2[i] = 0;
    }
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;

    // Wait for reset then turn LED off
//    while (digitalRead(resetPin) == HIGH);
//    digitalWrite(LED_BUILTIN, LOW);

    delay(500);

    // Prepare interrupts
    // Could this be causing issues???
    attachInterrupt(mic1, handler1, FALLING);
    attachInterrupt(mic2, handler2, FALLING);
}

