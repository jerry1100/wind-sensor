#define CYCLE ARM_DWT_CYCCNT

const int pin1 = 14;
const int pin2 = 15;
const int resetPin = 12;
volatile int startCycle;
volatile int endCycle;

void setup() {
    // Set up counting clock cycles
    ARM_DEMCR |= ARM_DEMCR_TRCENA;
    ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
    
    // Pin configurations
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    attachInterrupt(pin1, handler1, FALLING);
    pinMode(resetPin, INPUT_PULLUP);

    Serial.begin(9600);
}

void loop() {
}

void handler1() {
    #ifdef DEBUG
    Serial.printf("handler1\n");
    #endif
    
    startCycle = CYCLE;
    detachInterrupt(pin1);
    attachInterrupt(pin2, handler2, FALLING);
}

void handler2() {
    #ifdef DEBUG
    Serial.printf("handler2\n");
    #endif
    
    endCycle = CYCLE;
    detachInterrupt(pin2);
    waitForReset();
}

void waitForReset() {
    #ifdef DEBUG
    Serial.printf("wait for reset\n");
    #endif
    
    delayMicroseconds(100000);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf("%d\n", endCycle - startCycle);
    while (digitalRead(resetPin) == HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    
    #ifdef DEBUG
    Serial.printf("Restarting...\r\n");
    #endif
      
    attachInterrupt(pin1, handler1, FALLING);
}

