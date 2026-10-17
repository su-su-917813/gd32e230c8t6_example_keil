#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "LCD.h"
#include "touch_CTP.h"
		
int main(void)
{
	systick_config();  // ??? systick ?????
	LCD_GPIO__INIT();
	delay_1ms(200);       // ??????
	LCD_Initial();
	delay_1ms(200);
	Lcd_ColorBox(0, 0, 250, 350, WHITE);  // ???
	LCD_PutString(0,0,"Touch test start.",WHITE,BLACK,1);
	
	Touch_GPIO_Config();
	CTP_WakeUp();
	while (1) {
		Touch_Test();
  }
} 