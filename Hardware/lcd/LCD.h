#include <stdint.h>
#include <stdio.h>


/* GPIO Define */
#define GPIO_SPI GPIOA 
#define GPIO_SPI_CS GPIO_PIN_6 
#define GPIO_SPI_SCK GPIO_PIN_7 
#define GPIO_SPI_MOSI GPIO_PIN_8 
/*
#define SPI_CS(a)	\
						if (a)	\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_CS,1);	\
						else		\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_CS,0);
#define SPI_DCLK(a)	\
						if (a)	\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_SCK,1);	\
						else		\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_SCK,0);
#define SPI_SDA(a)	\
						if (a)	\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_MOSI,1);	\ 
						else		\
						gpio_bit_write(GPIO_SPI,GPIO_SPI_MOSI,0);
*/
#define SPI_CS(a)   do { if (a) gpio_bit_write(GPIO_SPI, GPIO_SPI_CS, 1); else gpio_bit_write(GPIO_SPI, GPIO_SPI_CS, 0); } while(0)
//#define SPI_DCLK(a) do { if (a) gpio_bit_write(GPIO_SPI, GPIO_SPI_SCK, 1); else gpio_bit_write(GPIO_SPI, GPIO_SPI_SCK, 0); } while(0)
#define SPI_DCLK(a) do { \
    if (a) gpio_bit_write(GPIO_SPI, GPIO_SPI_SCK, 1); \
    else gpio_bit_write(GPIO_SPI, GPIO_SPI_SCK, 0); \
    for(volatile int i=0; i<2; i++); \
} while(0)
#define SPI_SDA(a)  do { if (a) gpio_bit_write(GPIO_SPI, GPIO_SPI_MOSI, 1); else gpio_bit_write(GPIO_SPI, GPIO_SPI_MOSI, 0); } while(0)

#define WHITE						0xFFFF
#define BLACK						0x0000	  
#define BLUE						0x001F  
#define BRED						0XF81F
#define GRED						0XFFE0
#define GBLUE						0X07FF
#define RED							0xF800
#define MAGENTA						0xF81F
#define GREEN						0x07E0
#define CYAN						0x7FFF
#define YELLOW						0xFFE0
#define BROWN						0XBC40 //棕色
#define BRRED						0XFC07 //棕红色
#define GRAY						0X8430 //灰色




void LCD_GPIO__INIT(void);
void LCD_Initial(void); //LCD初始化函数
void LCD_delay(int time);
void WriteComm(unsigned char CMD);
void WriteData(unsigned short dat);
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp);
void Lcd_ColorBox(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint32_t Color);
void draw_Bat(uint16_t x,uint16_t y,uint8_t power_per);
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,unsigned char flag);
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, uint8_t* pic);
