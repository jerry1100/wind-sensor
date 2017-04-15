const int pin = 14; // pin 14 is at port D1 (port D, bit 1)

void setup() {
    pinMode(pin, OUTPUT);
}

void loop() {
    while (1) {
        //digitalWriteFast(pin, HIGH); // 13.7 MHz
        //__asm__ volatile("DSB");
        //digitalWriteFast(pin, LOW);
        digitalWrite(pin, HIGH); // 2.03 MHz
        __asm__ volatile("DSB");
        digitalWrite(pin, LOW);
    }
}
