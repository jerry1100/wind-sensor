IntervalTimer myTimer;

int period = 1000; // period is x ms
int pin = LED_BUILTIN;  // built-in LED
int state = LOW;
volatile int blinkCount = 1; // volatile type for sharing

void setup() {
  pinMode(pin, OUTPUT);
  myTimer.begin(blinkLED, period*500);  // call handler function every x us
  noInterrupts();
  Serial.begin(9600);
}

void loop() {
  int blinkCopy;
  noInterrupts(); // turn off interrupts for reading
  blinkCopy = blinkCount;
  interrupts(); // turn it back on asap

  Serial.printf("LED has blunk %d times\n", blinkCopy/2); // blink every two interrupts
  delay(period);
}

// Good idea to try and keep this as short as possible
void blinkLED() {
  state = ~state;
  blinkCount++;
  digitalWrite(pin, state);
}
