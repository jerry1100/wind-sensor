#define CYCLE ARM_DWT_CYCCNT

// These don't change
const int mic1 = 14;
//const int mic2 = 15;
const int resetPin = 12;
const int HI = 1;
const int LO = 0;

// Number of cycles to count up to (1 cycle = 2 triggers)
const int cycles = 5000;
const int triggers = 2*cycles;

// Counters and storage
int read1;
int cycle1;
int counter1;
int prev1;
unsigned start1;
unsigned end1;

//#define DEBUG

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Pin configurations
    pinMode(mic1, INPUT);
//    pinMode(mic2, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(resetPin, INPUT_PULLUP);

    // Set initial state
    counter1 = 0;
    prev1 = LO;

    Serial.begin(9600);
}

void loop() {
    read1 = analogRead(mic1);
    cycle1 = CYCLE;

    if (prev1 == LO && read1 > 771) { // triggered high
        prev1 = HI;

        if (counter1 == 0) {
            start1 = cycle1;
        }
        
        counter1++;
    } else if (prev1 == HI && read1 < 280) { // triggered low
        prev1 = LO;
        
        if (counter1 == 0) {
            start1 = cycle1;
        }

        counter1++;
    }

    if (counter1 == triggers) {
        end1 = cycle1;
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
    unsigned elapsed1 = end1 - start1;
    Serial.printf("Start: %u, Stop: %u, Elapsed: %u\n", start1, end1, elapsed1);
    Serial.printf("# Cycles: %d, Calculated Frequency: %.3lf Hz\n\n", cycles, (float) cycles*96000000/elapsed1);

    // Reset
    counter1 = 0;
    prev1 = LO;

    // Wait for reset then turn LED off
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif
}

