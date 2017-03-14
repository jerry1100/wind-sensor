#define NUM 1000

int cycles[NUM];
int i;
int prev;

void setup() {
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

  prev = ARM_DWT_CYCCNT;  
  for (i = 0; i < NUM; i++) {
    cycles[i] = ARM_DWT_CYCCNT - prev;
    prev = ARM_DWT_CYCCNT;
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }
  
  Serial.begin(9600);
}

void loop() {
  delay(1000);
  for (i = 0; i < NUM; i++) {
      Serial.printf("Cycle %d: %d\n", i, cycles[i]);
  }
  exit(0);
}
