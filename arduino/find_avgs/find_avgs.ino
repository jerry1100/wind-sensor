#define UP 1
#define DOWN 2

const int pin1 = 14;
const int pin2 = 15;
const int resetPin = 12;
int sum1 = 0;
int sum2 = 0;
int maxSum1 = 0;
int maxSum2 = 0;
int minSum1 = 0;
int minSum2 = 0;
int prev1 = -1;
int prev2 = -1;
int status1 = -1;
int status2 = -1;
int maxCount1 = 0;
int minCount1 = 0;
int maxCount2 = 0;
int minCount2 = 0;
int count = 0;

void setup() {
    Serial.begin(9600);
    pinMode(resetPin, INPUT_PULLUP);
    attachInterrupt(resetPin, resetHandler, FALLING);
}

void loop() {
    int read1 = analogRead(pin1);
    int read2 = analogRead(pin2);

    sum1 += read1;
    sum2 += read2;
    count++;

    if (prev1 != -1) {
        if (status1 == -1) {
            if (read1 > prev1) {
                status1 = UP;
            } else if (read1 < prev1) {
                status1 = DOWN;
            }
        } else if (status1 == UP) {
            if (read1 < prev1) { // if going up but now is going down, then prev is max
                maxSum1 += prev1;
                status1 = DOWN;
                maxCount1++;
            }
        } else if (status1 == DOWN) {
            if (read1 > prev1) { // if going down but now is going up, then prev is min
                minSum1 += prev1;
                status1 = UP;
                minCount1++;
            }
        }
    }
    prev1 = read1;

    if (prev2 != -1) {
        if (status2 == -1) {
            if (read2 > prev2) {
                status2 = UP;
            } else if (read2 < prev2) {
                status2 = DOWN;
            }
        } else if (status2 == UP) {
            if (read2 < prev2) { // if going up but now is going down, then prev is max
                maxSum2 += prev2;
                status2 = DOWN;
                maxCount2++;
            }
        } else if (status2 == DOWN) {
            if (read2 > prev2) { // if going down but now is going up, then prev is min
                minSum2 += prev2;
                status2 = UP;
                minCount2++;
            }
        }
    }
    prev2 = read2;
    
    Serial.printf("Read1 [min: %d, avg: %d, max: %d]\tRead2 [min: %d, avg: %d, max: %d]\n", 
        minSum1/minCount1, sum1/count, maxSum1/maxCount1, minSum2/minCount2, sum2/count, maxSum2/maxCount2);
}

void resetHandler() {
    sum1 = sum2 = 0;
    maxSum1 = maxSum2 = 0;
    minSum1 = minSum2 = 0;
    prev1 = prev2 = -1;
    status1 = status2 = -1;
    maxCount1 = maxCount2 = 0;
    minCount1 = minCount2 = 0;
    count = 0;
}

