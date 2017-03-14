#define ARRSIZE 500

int pin1 = 14;
int pin2 = 15;
int resetPin = 12;
//int lower = 460;
//int upper = 560;
unsigned short vals1[ARRSIZE];
unsigned short vals2[ARRSIZE];

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(resetPin, INPUT_PULLUP);
}

void loop() {
    int i;

    while(analogRead(pin1) < 700);

    // Read in data
    for (i = 0; i < ARRSIZE; i++) {
        vals1[i] = analogRead(pin1);
        vals2[i] = analogRead(pin2);
    }
    
    // Print out data
    int index = 0;
    for (i = 0; i < ARRSIZE; i++) {
        Serial.printf("%d,%d\r\n", vals1[i], vals2[i]);
        if (vals1[i] > 560 || vals1[i] < 460 || vals2[i] > 560 || vals2[i] < 460) {
            index = i;
        }
        vals1[i] = vals2[i] = 0; // reset array
    }

    // Wait for reset
    Serial.printf("Index: %d\n", index);
    digitalWrite(LED_BUILTIN, HIGH);
    while (digitalRead(resetPin) != LOW);
    Serial.printf("Restarting in 1 second\n");
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
}
