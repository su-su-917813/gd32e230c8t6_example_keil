#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "LCD.h"

		
int main(void)
{
	systick_config();  // ??? systick ?????
LCD_GPIO__INIT();
delay_1ms(200);       // ??????
LCD_Initial();
delay_1ms(200);
Lcd_ColorBox(0, 0, 240, 320, RED);  // ???
while(1);

} 