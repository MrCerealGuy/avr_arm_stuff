/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 
#include "LPC17xx.h"

int main(void)
{
  	LCD_Initializtion();
  	LCD_Clear(Red);

	GUI_Text(80,144,"Cortex M3",Cyan,Red);
	LCD_DrawLine(0,166,300,166,Magenta);

	GUI_Text(0,176,"Willkommen in der Welt der uC-Programmierung!",White,Red);

	while (1) { }
}

