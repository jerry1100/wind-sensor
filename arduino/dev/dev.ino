#include <TimerOne.h>

int pin = 14; // emitter pin
int state = HIGH; // 0 if low, 1 if high

void setup() {
  pinMode(pin, OUTPUT);
  Timer1.initialize(12);
  Timer1.attachInterrupt(toggle); // toggle pin every 25us
}

void loop() {

}

void pulse() {
  state = (state == HIGH) ? LOW : HIGH;
  digitalWrite(pin, state);
}