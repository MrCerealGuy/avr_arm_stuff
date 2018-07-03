#include <Manchester.h>

#define TX_PIN 5 // am Pin 5 wird der Sender angeschlossen
#define LED_PIN 13 // Pin für die blinkende LED

uint8_t moo = 1; // speichert den letzten LED-Status
uint16_t transmit_data = 0; // 16-Bit Zahl, die wir senden möchten

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, moo);

    man.setupTransmit(TX_PIN, MAN_1200);
}

void loop()
{
    man.transmit(transmit_data);
    transmit_data++; // Zahl inkrementieren

    moo = ++moo % 2;
    digitalWrite(LED_PIN, moo);
}