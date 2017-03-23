int readPin = 14;
int resetPin = 12;
volatile int max = 0;
volatile int min = 1023;

void setup() {
    Serial.begin(9600);
    pinMode(resetPin, INPUT_PULLUP);
    attachInterrupt(resetPin, resetHandler, FALLING);
}

void loop() {
    int val = analogRead(readPin);

    // Update current max and min
    max = (val > max) ? val : max;
    min = (val < min) ? val : min;
    
    Serial.printf("%d %d %d\n", val, max, min);
}

void resetHandler() {
    max = 0;
    min = 1023;
}

