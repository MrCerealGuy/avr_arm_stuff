#include <avr/io.h>

int main(void) {
	DDRB = 0xFF; // LED-Port auf Ausgang 
	DDRD = 0x00; // Key-Port auf Eingang

	PORTD = 0xFF; // die Pullup Widerstände aktivieren

	uint8_t old_key_state = 0xFF; // bisher war kein Taster gedrückt

	uint8_t new_key_state;
	uint8_t led_state = 0x00;
	uint8_t state_change = 0;

	while (1) {
		new_key_state = PIND;

		if (old_key_state != new_key_state) {

			old_key_state = new_key_state;
			state_change++;

			if (state_change == 2) {
				led_state = ~led_state;
				state_change = 0;
			}
		}

		PORTB = led_state;
	}
}