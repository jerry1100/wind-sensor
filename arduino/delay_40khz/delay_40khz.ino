const int pin = 14; // emitter pin

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
  send_pulses();
  delayMicroseconds(250);
}

void send_pulses() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(12);
    digitalWrite(pin, LOW);
    delayMicroseconds(12);
  }
}