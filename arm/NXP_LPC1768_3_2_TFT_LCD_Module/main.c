#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

volatile uint32_t temp;

void delay(uint32_t del)
{
    uint32_t i;

    for (i = 0; i < del; i++)
        temp = i;
}

void LEDBlinkExp()
{
    /* 10101010, d.h. von den Pins 0..7 werden wir 1, 3, 5 und 7 blinken lassen */
    unsigned long LED_PINS
        = ((uint32_t)1 << 1) | ((uint32_t)1 << 3) | ((uint32_t)1 << 5) | ((uint32_t)1 << 7);

    /* LEDs an PORT2 als Ausgang definieren  */
    GPIO_SetDir(2, LED_PINS, 1);

    while (1)
    {
        /* etwas warten */
        delay(1 << 24);

        /* PORT2 auf low setzen */
        GPIO_ClearValue(2, LED_PINS);

        /* etwas warten */
        delay(1 << 24);

        /* PORT2 auf high setzen */
        GPIO_SetValue(2, LED_PINS);
    }
}

int main(void)
{
    /* GPIO einschalten */
    LPC_SC->PCONP |= (1 << 15);

    /* GPIO Clock einschalten */
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

    LEDBlinkExp();

    return 0;
}