int pin1 = 14;
int pin2 = 15;
int resetPin = 12;
int lower = 460;
int upper = 560;
volatile int max1 = 0;
volatile int min1 = 1023;
volatile int max2 = 0;
volatile int min2 = 1023;

void setup() {
    Serial.begin(9600);
    pinMode(resetPin, INPUT_PULLUP);
    attachInterrupt(resetPin, resetHandler, FALLING);
}

void loop() {
    int read1 = analogRead(pin1);
    int read2 = analogRead(pin2);

    if (read1 > max1) max1 = read1;
    else if (read1 < min1) min1 = read1;
    if (read2 > max2) max2 = read2;
    else if (read2 < min2) min2 = read2;
    
    Serial.printf("%d %d %d\t%d %d %d\n", max1, read1, min1, max2, read2, min2);
}

void resetHandler() {
    max1 = max2 = 0;
    min1 = min2 = 1023;
}

