/*
 * main.c
 *
 * Created: 22.04.2012 16:44:06
 * Author: Andreas Zahnleiter
 */

#include <avr/io.h>
#include <util/delay.h > // benötigt für _delay_ms

int main(void) {
	uint8_t i = 0x00;

	DDRB = 0xff; // Port-B auf Ausgang legen

	do {
		PORTB = i; // Ausgabe Bitmuster
		i++;
		_delay_ms(900); // etwas warten
	}
	while (1);

	return 0;
}