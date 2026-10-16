#include "LCD.h"
#include "ASCII.h"
#include "GB1616.h"	//16*16汉字字模
#include "gd32e23x.h"

#include "systick.h"
#include <stdio.h>
#include <stdint.h>
void LCD_GPIO__INIT(void)
{
	/*
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOD, ENABLE);

	//                             BL背光开关       DC           CS           SCK          MISO        MOSI
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_CS(1);//初始化完GPIO，把引脚拉高，让液晶屏进入等待状态
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;	//LED 灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		//========== PA0 是五向按键的 中键 初始化为弱下接输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOC);
  gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);


	//gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6); //CS
  //gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7); //SCK
	//gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8); //MOSI
	gpio_mode_set(GPIO_SPI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_SPI_CS);
	gpio_mode_set(GPIO_SPI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_SPI_SCK);
	gpio_mode_set(GPIO_SPI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_SPI_MOSI);
  gpio_output_options_set(GPIO_SPI, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_SPI_CS);
	gpio_output_options_set(GPIO_SPI, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_SPI_SCK);
	gpio_output_options_set(GPIO_SPI, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_SPI_MOSI);
	
	SPI_CS(1);//初始化完GPIO，把引脚拉高，让液晶屏进入等待状态

}
void LCD_delay(int time)  //延时函数
{
	volatile  unsigned short i,j;
	for(i=0;i<time;i++)
		for(j=0;j<1000;j++)	;
}
void LCD_WriteByteSPI(unsigned char byte) //SPI模拟函数，写一个8bit的数据
{
	  unsigned char buf;
    unsigned char i;
    for(i=0;i<8;i++) 
    {
        buf=(byte>>(7-i))&0x1;
				SPI_DCLK(0);
        SPI_SDA(buf);
        SPI_DCLK(1);
    }	
}
void WriteComm(unsigned char CMD) //写命令
{
	gpio_bit_write(GPIOC, GPIO_PIN_13,1);
	SPI_CS(0);	
	SPI_SDA(0);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(CMD);
	SPI_CS(1);
	gpio_bit_write(GPIOC, GPIO_PIN_13,0);    // ??

}

void WriteData(unsigned short dat) //写数据
{
	SPI_CS(0);	
	SPI_SDA(1);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(dat);
	SPI_CS(1);
}
/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp)
{
	WriteComm(Index);
	WriteData(CongfigTemp);
}
/**********************************************
函数名：开窗函数

入口参数：XStart x方向的起点
          Xend   x方向的终点
					YStart y方向的起点
          Yend   y方向的终点

这个函数的意义是：开一个矩形框，方便接下来往这个框填充数据
***********************************************/
/*
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	WriteComm(0x2a);
	WriteData(Xstart>>8);
	WriteData(Xstart);
	WriteData(Xend>>8);
	WriteData(Xend);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart);
	WriteData(Yend>>8);
	WriteData(Yend);
	
	WriteComm(0x2c);

}
*/
void BlockWrite(unsigned int Xstart, unsigned int Xend, unsigned int Ystart, unsigned int Yend)
{
    // ????????? 0x2A
    SPI_CS(0);
    SPI_SDA(0); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(0x2A);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Xstart>>8);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Xstart);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Xend>>8);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Xend);

    // ????????? 0x2B
    SPI_SDA(0); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(0x2B);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Ystart>>8);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Ystart);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Yend>>8);
    SPI_SDA(1); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(Yend);

    // ??? RAM ?? 0x2C,????? CS
    SPI_SDA(0); SPI_DCLK(0); SPI_DCLK(1); LCD_WriteByteSPI(0x2C);
    // ??:????? SPI_CS(1),?? CS ?
}

/**********************************************
函数名：Lcd矩形填充函数

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint32_t Color)
{
	uint32_t i,j;
	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	SPI_CS(0);
	j = xLong*yLong;
	for (i=0; i<j; i++)
	{
		SPI_SDA(1);
		SPI_DCLK(0);
		SPI_DCLK(1);
		LCD_WriteByteSPI(Color>>8);
		SPI_SDA(1);
		SPI_DCLK(0);
		SPI_DCLK(1);
		LCD_WriteByteSPI(Color);
	}
	SPI_CS(1);
}
void LCD_Initial(void) //LCD初始化函数
{
	

	//************* Start Initial Sequence **********//
	WriteComm(0x11);     
  
	delay_1ms(120);

	WriteComm(0x36);     
	WriteData(0x00);   

	WriteComm(0x3A);     
	WriteData(0x06);   

	WriteComm(0xB2);     
	WriteData(0x1F);   
	WriteData(0x1F);   
	WriteData(0x00);   
	WriteData(0x33);   
	WriteData(0x33);  

	WriteComm(0xB7);    
	WriteData(0x12);  //VGH=12.54V,VGL=-8.23V 

	WriteComm(0xBB);     
	WriteData(0x66);  

	WriteComm(0xC0);     
	WriteData(0x2C); 

	WriteComm(0xC2);     
	WriteData(0x01);   

	WriteComm(0xC3);     
	WriteData(0x15); //4.6V 

	WriteComm(0xC4);     
	WriteData(0x20);   //VDV, 0x20:0v

	WriteComm(0xC6);     
	WriteData(0x13);     

	WriteComm(0xD0);     
	WriteData(0xA4);   
	WriteData(0xA1);   

	WriteComm(0xD6);     
	WriteData(0xA1);   //sleep in后，gate输出为GND

	WriteComm(0xE0);
	WriteData(0xF0);
	WriteData(0x06);
	WriteData(0x0D);
	WriteData(0x0B);
	WriteData(0x0A);

	WriteData(0x07);
	WriteData(0x2E);
	WriteData(0x43);
	WriteData(0x45);
	WriteData(0x38);
	WriteData(0x14);
	WriteData(0x13);
	WriteData(0x25);
	WriteData(0x29);

	WriteComm(0xE1);
	WriteData(0xF0);
	WriteData(0x07);
	WriteData(0x0A);
	WriteData(0x08);
	WriteData(0x07);
	WriteData(0x23);
	WriteData(0x2E);
	WriteData(0x33);
	WriteData(0x44);
	WriteData(0x3A);
	WriteData(0x16);
	WriteData(0x17);
	WriteData(0x26);
	WriteData(0x2C);  

	WriteComm(0x3A); //65k mode
	WriteData(0x55); //WriteData(0x05);
	WriteComm(0x36); //Set_address_mode 这个寄存器用于旋转屏幕
	WriteData(0x00); //横屏，从左下角开始，从左到右，从下到上

	WriteComm(0x20);
	WriteComm(0x29); //Display on
					

}

/******************************************
函数名：Lcd图像填充
功能：向Lcd指定位置填充图像
入口参数：
					(x,y): 图片左上角起始坐标
					(pic_H,pic_V): 图片的宽高
					 pic  指向存储图片数组的指针
******************************************/
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, uint8_t* pic)
{	
	//========= 轮询方式=========//
  unsigned long i;
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	SPI_CS(0);
	for (i = 0; i < pic_H*pic_V*2; i++)
	{
		SPI_SDA(1);
		SPI_DCLK(0);
		SPI_DCLK(1);
		LCD_WriteByteSPI(pic[i]);
	}
	SPI_CS(1);	
}
//=============== 在x，y 坐标上打一个颜色为Color的点 ===============
void DrawPixel(uint16_t x, uint16_t y, int Color)
{
	BlockWrite(x,x,y,y); 
  SPI_CS(0);
	SPI_SDA(1);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(Color>>8);
	SPI_SDA(1);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(Color);
	SPI_CS(1);						  
}
void LCD_draw_VLine(uint16_t x,uint16_t y0,uint16_t y1,uint16_t Color)
{
	uint16_t i;
	BlockWrite(x,x,y0,y1);
	SPI_CS(0);
	for(i=0;i<y1-y0+1;i++)
		{
			SPI_SDA(1);
			SPI_DCLK(0);
			SPI_DCLK(1);
			LCD_WriteByteSPI(Color>>8);
			SPI_SDA(1);
			SPI_DCLK(0);
			SPI_DCLK(1);
			LCD_WriteByteSPI(Color); 
		}
	SPI_CS(1);
}
void LCD_draw_HLine(uint16_t x0,uint16_t x1,uint16_t y,uint16_t Color)
{
	uint16_t i;
	BlockWrite(x0,x1,y,y);
	SPI_CS(0);
	for(i=0;i<x1-x0+1;i++)
		{
			SPI_SDA(1);
			SPI_DCLK(0);
			SPI_DCLK(1);
			LCD_WriteByteSPI(Color>>8);
			SPI_SDA(1);
			SPI_DCLK(0);
			SPI_DCLK(1);
			LCD_WriteByteSPI(Color);  
		}
	SPI_CS(1);
}
//电池电量，x,y是电池显示位置，power_per是电池显示的百分比，范围为0~4，
void draw_Bat(uint16_t x,uint16_t y,uint8_t power_per)
{
	uint8_t i,j;
	LCD_draw_VLine(x,y,y+5,BLACK);
	LCD_draw_VLine(x,y+5+10,y+5+10+5,BLACK);
	
	LCD_draw_HLine(x,x+28,y,BLACK);
	LCD_draw_HLine(x,x+28,y+20,BLACK);
	
	LCD_draw_VLine(x+28,y,y+5,BLACK);
	LCD_draw_VLine(x+28,y+5+10,y+5+10+5,BLACK);
	
	LCD_draw_HLine(x+28,x+28+3,y+5,BLACK);
	LCD_draw_HLine(x+28,x+28+3,y+5+10,BLACK);
	
	LCD_draw_VLine(x+28+3,y+5,y+5+10,BLACK);
	
	for(j=0;j<power_per;j++)
	{
		BlockWrite(x+3,x+3+4,y+3,y+3+14);
		SPI_CS(0);
		for(i=0;i<6*15;i++)
			{ 
				SPI_SDA(1);
				SPI_DCLK(0);
				SPI_DCLK(1);
				LCD_WriteByteSPI(BLACK>>8);
				SPI_SDA(1);
				SPI_DCLK(0);
				SPI_DCLK(1);
				LCD_WriteByteSPI(BLACK);			
			}
		SPI_CS(1);
		x=x+6;
	}
}
/**********8*16字体 ASCII码 显示*************
(x,y): 显示字母的起始坐标
num:   要显示的字符:" "--->"~"
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/

void SPILCD_ShowChar(unsigned short x,unsigned short y,unsigned char num, unsigned int fColor, unsigned int bColor,unsigned char flag) 
{       
	unsigned char temp;
	unsigned int pos,i,j;  

	num=num-' ';//得到偏移后的值
	i=num*16; 	
	for(pos=0;pos<16;pos++)
		{
			temp=nAsciiDot[i+pos];	//调通调用ASCII字体
			for(j=0;j<8;j++)
		   {                 
		        if(temp&0x80)
							DrawPixel(x+j,y,fColor);
						else 
							if(flag) DrawPixel(x+j,y,bColor); //如果背景色标志flag为1
							temp<<=1; 
		    }
			 y++;
		}		 
}  

/**********写一个16x16的汉字*****************
(x,y): 显示汉字的起始坐标
c[2]:  要显示的汉字
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/

void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j,k;
	unsigned short m;
	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
	  if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1]))
			{ 
    	for(i=0;i<32;i++) 
			{
				m=codeGB_16[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if(i%2){y++;x=x-8;}
				else x=x+8;
		  }
		}  
	  }	
	}

/**********显示一串字*****************
(x,y): 字符串的起始坐标
*s:    要显示的字符串指针
fColor 前景色
bColor 背景色
flag:  有背景色(1)无背景色(0)
*********************************************/
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
	{
		unsigned char l=0;
		while(*s) 
			{
				if( (unsigned char)*s < 0x80) 
						{
							SPILCD_ShowChar(x+l*8,y,*s,fColor,bColor,flag);
							s++;l++;
						}
				
				else
						{
							PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
							s+=2;l+=2;
						}
			  
			}
	}
