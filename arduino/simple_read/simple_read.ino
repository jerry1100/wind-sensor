int samples = 1000;
int values[1000];
int counter = 0;
const int read_pin = 15;

// Setup modules, executed once
void setup() {

  Serial.begin(9600);
  Serial.printf("Serial port has begun!\n");  
}

// Main loop, run repeatedly
void loop() {
  if (counter < samples) { // read data
    values[counter++] = analogRead(read_pin);
  } else { // print data and quit
    int i;
    for (i = 0; i < samples; i++) {
      //Serial.printf("%kld, %d\n", i, values[i]);
      Serial.printf("%d\n", values[i]);
    }
    exit(0);
  }
  
  delay(1);
}
