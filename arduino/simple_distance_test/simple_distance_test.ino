#include <TimerOne.h>

const int pin1 = 14;
const int pin2 = 15;
const int val1 = 512;
const int val2 = 512;
const int lower = 450;
const int upper = 550;
const int resetPin = 12;
int state = 0;
volatile int counter = 0; // volatile type for sharing

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(resetPin, INPUT_PULLUP);
  
  Timer1.initialize(1);
  Timer1.attachInterrupt(count);
  Timer1.stop();
  Serial.begin(9600);
}

void loop() {
  switch (state) {
    case 0: // wait for first microphone
      val1 = analogRead(pin1);
      state += (val1 < lower || val1 > upper);
      break;
    case 1:
      Timer1.restart(); // start counting
      state += 1;
    case 2: // wait for second microphone
      val2 = analogRead(pin2);
      state += (val2 < lower || val2 > upper);
      break;
    case 3:
      Timer1.stop();
      Serial.printf("%d us has elapsed\n", counter);

      digitalWrite(LED_BUILTIN, HIGH);
      while (digitalRead(resetPin) == HIGH);
      digitalWrite(LED_BUILTIN, LOW);

      state = 0;
      counter = 0;
      break;
  }
}

void count() {
  counter++;
}
