#define CYCLE ARM_DWT_CYCCNT

const int micPin = 14;
const int resetPin = 12;
const int numCycles = 1000;
const int numTriggers = 2*numCycles;
volatile int startCycle;
volatile int endCycle;
volatile int counter;

//#define DEBUG

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    counter = 0;
    
    // Pin configurations
    pinMode(micPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    attachInterrupt(micPin, handler1, FALLING);
    pinMode(resetPin, INPUT_PULLUP);

    Serial.begin(9600);
}

void loop() {
}

void handler1() {
    #ifdef DEBUG
    Serial.printf("handler1\n");
    #endif

    if (counter == numTriggers) {
        endCycle = CYCLE;
        detachInterrupt(micPin);
        waitForReset();
    } else if (counter == 0) {
        startCycle = CYCLE;
        counter++;
    } else {
        counter++;
    }
}

void waitForReset() {
    #ifdef DEBUG
    Serial.printf("Waiting for reset\n");
    #endif
    
//    delayMicroseconds(100000);
    digitalWrite(LED_BUILTIN, HIGH);
    long unsigned elapsed = endCycle - startCycle;
    Serial.printf("Start: %lu, Stop: %lu, Elapsed: %lu\n", startCycle, endCycle, elapsed);
    Serial.printf("# Cycles: %d, Calculated Frequency: %.3lf Hz\n\n", numCycles, (float) numCycles*96000000/elapsed);
    
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    counter = 0;
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif

    attachInterrupt(micPin, handler1, FALLING);
}

