int led = 3;
int level = 0;
int steps = 1;

// Initial setup to run once
void setup() {
  pinMode(led, level);
}

// Main loop to run repeatedly
void loop() {
  Serial.printf("Current brightness: %d\n", level);
  
  analogWrite(led, level);
  level += steps;

  if (level == 0 || level == 255) {
    steps = -steps;
  }

  delay(5);
}
