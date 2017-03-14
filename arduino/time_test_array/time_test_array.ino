#define CYCLE ARM_DWT_CYCCNT

// These don't change
const int mic1 = 14;
const int mic2 = 15;
const int resetPin = 12;
const int margin = 35000;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 10;
const int triggers = 2*cycles;

// Variables used in interrupts
volatile int counter1;
volatile int counter2;
volatile int finished1;
volatile int finished2;
volatile int arr1[triggers];
volatile int arr2[triggers];
volatile int nums1[triggers];
volatile int nums2[triggers];
volatile int *p1;
volatile int *p2;
volatile unsigned start1;
volatile unsigned start2;

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

    // Set last 2 elements in array
    p1 = arr1;
    p2 = arr2;

    Serial.begin(9600);
}

void loop() {
    // Wait until both are ready
    while (!finished1 || !finished2) delay(1000);
    waitForReset();
}

void handler1() {
    // Beginning
    start1 = CYCLE;
    #ifdef DEBUG
    Serial.printf("Handler1 in\n");
    #endif

    // Check if we're done
    if (counter1 == triggers) {
        finished1 = 1;
        detachInterrupt(mic1);
        #ifdef DEBUG
        Serial.printf("Handler1 stopped\n");
        #endif
    }
    
    // Save current cycle into array
    nums1[counter1] = CYCLE;
    arr1[counter1++] = CYCLE - start1;
}

void handler2() {
    start2 = CYCLE;
    #ifdef DEBUG
    Serial.printf("Handler2 in\n");
    #endif
    
    // Check if we're done
    if (counter2 == triggers) {
        finished2 = 1;
        detachInterrupt(mic2);
        #ifdef DEBUG
        Serial.printf("Handler2 stopped\n");
        #endif
    }

    nums2[counter2] = CYCLE;
    arr2[counter2++] = CYCLE - start2;
}

void waitForReset() {
    #ifdef DEBUG
    Serial.printf("Waiting for reset\n");
    #endif

    // Turn LED on to show that we're done
    delayMicroseconds(100000);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.printf("===========================================================================\n");
    Serial.printf("RAW DATA ARR1 (IN ORDER):\n");
    int sum1 = 0;
    for (int i = 0; i < triggers; i++) {
        Serial.printf("%u\n", arr1[i]);
        sum1 += arr1[i];
    }
    Serial.printf("The average of %d differences is: %.3f\n", triggers, (float) sum1/triggers);

    Serial.printf("\nRAW DATA ARR2 (IN ORDER):\n");
    int sum2 = 0;
    for (int i = 0; i < triggers; i++) {
        Serial.printf("%u\n", arr2[i]);
        sum2 += arr2[i];
    }
    Serial.printf("The average of %d differences is: %.3f\n", triggers, (float) sum2/triggers);

    Serial.printf("===========================================================================\n");
//    p1 = arr1;
//    p2 = arr2;
//    while (p1 < arr1 + triggers || p2 < arr2 + triggers) { // while not at end of array
//        if (*p1 < *p2 && p1 < arr1 + triggers) {
//            if (p1 == arr1) {
//                Serial.printf("P1: %lu\n", *p1);
//            } else {
//                Serial.printf("P1: %lu %lu\n", *p1, *p1 - *(p1-1));
//            }
//            p1++;
//        } else if (p2 < arr2 + triggers) {
//            if (p2 == arr2) {
//                Serial.printf("P2: %lu\n", *p2);
//            } else {
//                Serial.printf("P2: %lu %lu\n", *p2, *p2 - *(p2-1));
//            }
//            p2++;
//        }
//    }
   
    // Clear and reset
    int i;
    for (i = 0; i < triggers; i++) {
        arr1[i] = 0;
        arr2[i] = 0;
    }
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;

    // Wait for reset then turn LED off
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif

    attachInterrupt(mic1, handler1, FALLING);
    attachInterrupt(mic2, handler2, FALLING);
}

