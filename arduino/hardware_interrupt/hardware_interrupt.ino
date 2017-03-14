#include <TimerOne.h>

int pin1 = 14;
int pin2 = 15;
int state = 0;
volatile int counter = 0; // volatile type for sharing

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  
  attachInterrupt(pin1, handler1, FALLING);
  attachInterrupt(pin2, handler2, FALLING);
  
  Timer1.initialize(1);
  Timer1.attachInterrupt(count);
  Timer1.stop();
  
  Serial.begin(9600);
}

void loop() {
  int copyCounter;
  
  noInterrupts();
  copyCounter = counter;
  interrupts();

  Serial.printf("%d\n", copyCounter);
  delay(1000);
  
}

void handler1() { // start counting
  if (state == 0) {
    state = 1;
    Timer1.start();
    detachInterrupt(pin1);
  }
}

void handler2() { // stop counting
  if (state == 1) {
    state = 2;
    Timer1.stop();
    detachInterrupt(pin2);
  }
}

void count() {
  counter++;
}
