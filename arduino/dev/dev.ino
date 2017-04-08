int pin = 14; // emitter pin

void setup()
{
    pinMode(pin, OUTPUT);
}

void loop()
{
    pulse();
    delayMicroseconds(1600);
}

void pulse()
{
    for (int i = 0; i < 100; i++)
    {
        digitalWrite(pin, HIGH);
        delayMicroseconds(12);
        digitalWrite(pin, LOW);
        delayMicroseconds(12);
    }
}