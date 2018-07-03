#include <LiquidCrystal.h>
#include <Manchester.h>

#define RX_PIN 30 // Signal am Pin 30 abgreifen
#define LED_PIN 13

uint8_t moo = 1;

// LCD intialisieren
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, moo);

    // Empfänger einrichten
    man.setupReceive(RX_PIN, MAN_1200);
    man.beginReceive();

    // LCD einrichten, 16 Spalten, 2 Zeilen
    lcd.begin(16, 2);
}

void loop()
{
    if (man.receiveComplete()) // auf Daten warten
    {
        uint16_t m = man.getMessage();
        man.beginReceive();

        moo = ++moo % 2;
        digitalWrite(LED_PIN, moo);

        // Zahl über LCD ausgeben
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(m);
    }
}