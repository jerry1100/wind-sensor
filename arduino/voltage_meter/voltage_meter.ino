const int pin = 16; // pin the probe is on
const int res = 1023; // resolution of adc

void setup() {
    Serial.begin(9600);}

void loop() {
    int val = analogRead(pin);
    Serial.printf("ADC: %d, Voltage: %f\n", val, (float) val*3.3/1023.0);
    delay(500);
}
