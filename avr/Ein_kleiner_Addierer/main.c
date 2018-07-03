/*
 * main.c
 *
 * Addiert zwei Zahlen und gibt das Ergebnis über eine LED aus.
 *
 * Created: 01.05.2012
 * Author: Andreas Zahnleiter
 */

#ifndef F_CPU
#define F_CPU 3686400
#endif

#include <avr/io.h>
#include <util/delay.h> // benötigt für _delay_ms
#include <inttypes.h>

/* Einfache Funktion zum Entprellen eines Tasters */
inline uint8_t debounce(volatile uint8_t* port, uint8_t pin)
{
    if (!(*port & (1 << pin))) {
        /* Pin wurde auf Masse gezogen, 100ms warten */
        _delay_ms(100);

        if (*port & (1 << pin)) {
            /* Anwender Zeit zum Loslassen des Tasters geben */
            _delay_ms(100);
            return 1;
        }
    }
    return 0;
}

/* Legt ein Signal auf einen Ausgang mit definierter Dauer */
void signal(volatile uint8_t* port, uint8_t pin, uint8_t delay)
{
    *port = *port ^ (1 << pin);
    _delay_ms(delay);
    *port = *port ^ (1 << pin);
}

/* Gibt die Summe über eine LED aus */
void ausgabe(uint8_t summe)
{
    uint8_t i = 0x00;

    _delay_ms(200);

    do {
        signal(&PORTB, PB0, 200);
        _delay_ms(1000);
        i++;

    } while (i < summe);
}

int main(void)
{
    uint8_t zahl1 = 0x00;
    uint8_t zahl2 = 0x00;
    uint8_t state = 0x00; // 0 = 1. Summanden einlesen, 1 = 2. Summanden
    // einlesen, 2 = addieren und ausgeben

    DDRB = 0xff; // Port-B auf Ausgang legen
    DDRD = 0x00; // Port-D auf Eingang legen
    PORTD = 0xff; // Pull-Ups aktivieren

    do {
        // Zähler-Taste gedrückt?
        if (debounce(&PIND, PD2)) {
            switch (state) {
            case 0: // 1. Summanden inkrementieren
                zahl1++;
                signal(&PORTB, PB0, 200);
                break;

            case 1: // 2. Summanden inkrementieren
                zahl2++;
                signal(&PORTB, PB0, 200);
                break;
            }
        }

        // Kommando-Taste gedrückt?
        if (debounce(&PIND, PD3)) {
            state++;

            signal(&PORTB, PB1, 1000);

            if (state == 2) {
                ausgabe(zahl1 + zahl2);
                state = zahl1 = zahl2 = 0;
            }
        }
    } while (1);

    return 0;
}