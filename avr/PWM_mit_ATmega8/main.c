#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << DDB3); // PB3 auf Ausgang legen

    OCR2 = 128; // Pulsbreite auf 50% setzen

    TCCR2 |= (1 << COM21); // nicht-invertierender Modus
    TCCR2 |= (1 << WGM21) | (1 << WGM20); // Fast PWM
    TCCR2 |= (1 << CS21); // Timer Vorteiler auf 8 setzen

    while (1)
    {
    }
}