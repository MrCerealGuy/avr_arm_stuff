int ledCount = 6; // Anzahl der Leds

int leds[6] = { 9, 8, 7, 6, 5, 4 }; // Pins der LEDs

int potiPin = A13; // Analoger Pin für Poti zur Einstellung der Intensität
int audioPin = A0; // Analoger Pin für Audio-Eingang

int i;

void setup()
{
    pinMode(potiPin, INPUT);
    pinMode(audioPin, INPUT);

    for (i = 0; i < ledCount; i++)
        pinMode(leds[i], OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    int intensity = analogRead(potiPin);
    int audio = analogRead(audioPin);

    Serial.println("Audio: " + String(audio));
    Serial.println("Intensitaet: " + String(intensity));

    audio = audio / intensity;

    Serial.println("Pegel: " + String(audio));

    if (audio == 0) // Wenn Lautstärke = 0 dann alle LEDs ausschalten
        for (i = 0; i < ledCount; i++)
            digitalWrite(leds[i], LOW);
    else
    {
        for (i = 0; i < audio;
             i++) // LEDs einschalten, je höher die Lautstärke, desto mehr LEDs werden eingeschaltet
            digitalWrite(leds[i], HIGH);

        for (i = i; i < ledCount; i++) // LEDs ausschalten, je niedriger die Lautstärke, desto mehr
                                       // LEDs werden ausgeschaltet
            digitalWrite(leds[i], LOW);
    }

    delay(100);
}