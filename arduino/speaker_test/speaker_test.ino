#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

AudioSynthWaveformSine   sine1;          
AudioOutputAnalog        dac1;           
AudioConnection          patchCord2(sine1, 0, dac1, 0);

long freq = 400;

void setup() {
  AudioMemory(20);
  sine1.frequency(freq);
  sine1.amplitude(3.3);
}


void loop(){  
}
