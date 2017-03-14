#include <TimerOne.h>

int pin1 = 14;
int pin2 = 15;
int resetPin = 12;
volatile int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  attachInterrupt(pin1, handler1, FALLING);
  
  Timer1.initialize(1);
  Timer1.attachInterrupt(count);
  Timer1.stop();
  
  Serial.begin(9600);
}

void loop() {
}

void count() { // increment every us
  counter++;
}

void handler1() { // start counting
  Timer1.restart();
  attachInterrupt(pin2, handler2, FALLING);
  detachInterrupt(pin1);
}

void handler2() { // stop counting
  Timer1.stop();
  detachInterrupt(pin2);
  cleanup();
}

void cleanup() {
  Serial.printf("%d\n", counter);
  counter = 0;

  digitalWrite(LED_BUILTIN, HIGH);
  while (digitalRead(resetPin) == HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  attachInterrupt(pin1, handler1, FALLING);
}
