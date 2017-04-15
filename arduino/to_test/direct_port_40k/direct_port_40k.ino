#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
  cli(); // disable interrupts
}

void loop() {
}

void toggle() {
  state = (state == HIGH) ? LOW : HIGH;
  digitalWrite(pin, state);
}