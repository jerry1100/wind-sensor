# Wind Sensor

## List of things to do
- Use a timer and measure the amount of time it takes for the arduino to send 10 pulses (10 up and 10 downs). It should be ~250us since each pulse is 25us.
- 

## Timing
| Operation | # Cycles | Time |
| -------------- |:--------:|:----------:|
| NOP | 1 | 10.42 ns |
| analogRead() | 870 | 9.06 us |
| digitalWrite() | 23 | 239.58 ns |
