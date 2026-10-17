
#ifndef  __TOUCH_CTP_H__
#define  __TOUCH_CTP_H__
#include <stdint.h>
#include "systick.h"
#include "gd32e23x.h"
#include <stdio.h>


/*
#define I2C1_SCL(x) 	\
						if (x)	\
						GPIOA->BSRR = GPIO_Pin_2;	\
						else		\
						GPIOA->BRR  = GPIO_Pin_2;
						
#define I2C1_SDA(x)	\
						if (x)	\
						GPIOA->BSRR = GPIO_Pin_3;	\
						else		\
						GPIOA->BRR  = GPIO_Pin_3;
*/
#define I2C1_SCL(x)   do { if (x) gpio_bit_write(GPIO_I2C1, GPIO_I2C1_SCL, 1); else gpio_bit_write(GPIO_I2C1, GPIO_I2C1_SCL, 0); } while(0)
#define I2C1_SDA(x) do { if (x) gpio_bit_write(GPIO_I2C1, GPIO_I2C1_SDA, 1); else gpio_bit_write(GPIO_I2C1, GPIO_I2C1_SDA, 0); } while(0)

#define	I2C1_SDA_Read()  	  	SDA_read_Bit()   
#define	I2C1_SDA_set_out();  	gpio_mode_set(GPIO_I2C1, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_I2C1_SDA); //GPIOA->CRL = (GPIOA->CRL&0xffff0fff)|0x00003000;
#define	I2C1_SDA_set_in();  	gpio_mode_set(GPIO_I2C1, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_I2C1_SDA); //GPIOA->CRL = (GPIOA->CRL&0xffff0fff)|0x00008000;

#define GPIO_I2C1 GPIOB
#define GPIO_I2C1_SCL GPIO_PIN_3
#define GPIO_I2C1_SDA GPIO_PIN_4						
						
#define CTP_ADDR				0x70		//地址为0x38要移一位 
						
#define COMMAND_BUFFER_INDEX         		0x20
#define QUERY_BUFFER_INDEX           		0x80
#define COMMAND_RESPONSE_BUFFER_INDEX		0xA0
#define POINT_BUFFER_INDEX           		0xE0
#define QUERY_SUCCESS                		0x00
#define QUERY_BUSY                     	0x01
#define QUERY_ERROR                     0x02
#define QUERY_POINT 
						
#define POINT           0x08
#define GESTURES        0x80

#define TAP             0x20
#define FLICK           0x22
#define DOUBLE_TAP      0x23

#define UP              0x08
#define UPPER_RIGHT     0x09
#define RIGHT           0x0A
#define LOWER_RIGHT     0x0B
#define DOWN            0x0C
#define LOWER_LEFT      0x0D
#define LEFT						0x0E
#define UPPER_LEFT			0x0F
						
void Touch_GPIO_Config(void); 
int SDA_read_Bit(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Ack(void);
void I2C1_NoAck(void);

void I2C1_Send_Byte(uint8_t dat);
uint8_t I2C1_Read_Byte(uint8_t ack);
uint8_t I2C1_WaitAck(void);


static uint8_t CTP_Write_Reg(uint8_t startaddr,uint8_t *pbuf,uint32_t len);
uint8_t CTP_Read_Reg(uint8_t *pbuf,uint32_t len);

int GUI_TOUCH_X_MeasureX(void); 
int GUI_TOUCH_X_MeasureY(void);

void Touch_Test(void);
void CTP_WakeUp(void);
#endif                                     

