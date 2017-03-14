#define CYCLE ARM_DWT_CYCCNT

// Pin declarations
const int mic1 = 14;
const int mic2 = 15;
const int resetPin = 12;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 1000;
const int triggers = 2*cycles;

// Variables used in interrupts
volatile int start1;
volatile int start2;
volatile int end1;
volatile int end2;
volatile int counter1;
volatile int counter2;
volatile int finished1;
volatile int finished2;

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
    while (!finished1 || !finished2) delay(1000);
    waitForReset();
}

void handler1() {
    #ifdef DEBUG
    Serial.printf("Handler1 in\n");
    #endif

    if (counter1 == triggers) {
        end1 = CYCLE;
        finished1 = 1;
        detachInterrupt(mic1);
        #ifdef DEBUG
        Serial.printf("Handler1 stopped\n");
        #endif
    } else if (counter1 == 0) {
        start1 = CYCLE;
        counter1++;
        #ifdef DEBUG
        Serial.printf("Handler1 started\n");
        #endif
    } else {
        counter1++;
        #ifdef DEBUG
        Serial.printf("Handler1 add\n");
        #endif
    }
}

void handler2() {
    #ifdef DEBUG
    Serial.printf("Handler2 in\n");
    #endif

    if (counter2 == triggers) {
        end2 = CYCLE;
        finished2 = 1;
        detachInterrupt(mic2);
        #ifdef DEBUG
        Serial.printf("Handler2 stopped\n");
        #endif
    } else if (counter2 == 0) {
        start2 = CYCLE;
        counter2++;
        #ifdef DEBUG
        Serial.printf("Handler2 started\n");
        #endif
    } else {
        counter2++;
        #ifdef DEBUG
        Serial.printf("Handler2 add\n");
        #endif
    }
}

void waitForReset() {
    #ifdef DEBUG
    Serial.printf("Waiting for reset\n");
    #endif
    
    delayMicroseconds(100000);

    // Print results
    digitalWrite(LED_BUILTIN, HIGH);
    long unsigned elapsed1 = end1 - start1;
    long unsigned elapsed2 = end2 - start2;
    Serial.printf("Start: %lu, Stop: %lu, Elapsed: %lu\n", start1, end1, elapsed1);
    Serial.printf("# Cycles: %d, Calculated Frequency: %.3lf Hz\n\n", cycles, (float) cycles*96000000/elapsed1);
    Serial.printf("Start: %lu, Stop: %lu, Elapsed: %lu\n", start2, end2, elapsed2);
    Serial.printf("# Cycles: %d, Calculated Frequency: %.3lf Hz\n\n\n", cycles, (float) cycles*96000000/elapsed2);

    // Wait for reset
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    counter1 = counter2 = 0;
    finished1 = finished2 = 0;
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif

    attachInterrupt(mic1, handler1, FALLING);
    attachInterrupt(mic2, handler2, FALLING);
}

