#define CYCLE ARM_DWT_CYCCNT

const int pin1 = 14;
const int pin2 = 15;
const int resetPin = 12;
const int upperThresh1 = 800;
const int lowerThresh1 = 280;
const int upperThresh2 = 610;
const int lowerThresh2 = 380;

//820 264
//633 408

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Pin configurations
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(resetPin, INPUT_PULLUP);
    
    Serial.begin(9600);
}

void loop() {  
    unsigned short val1;
    unsigned short val2;
    unsigned long startCycle;
    unsigned long endCycle;
    
    // Wait for mic1 to hear
    do {
        val1 = analogRead(pin1);
    } while (val1 > lowerThresh1 && val1 < upperThresh1);
    startCycle = CYCLE;
    
    Serial.printf("Triggered\n");

    // Wait for mic2 to hear
    do {
        val2 = analogRead(pin2);
    } while (val2 > lowerThresh2 && val2 < upperThresh2);
    endCycle = CYCLE;

    // Wait until reset
    Serial.printf("Val1: %d, Val2: %d\n", val1, val2);
    Serial.printf("Diff: %lu, Start: %lu, End: %lu\n\n", 
        endCycle - startCycle, startCycle, endCycle);
    digitalWrite(LED_BUILTIN, HIGH);
    while (digitalRead(resetPin) == HIGH);
    Serial.printf("Starting in 1 second\n");
    delay(1000);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
}
