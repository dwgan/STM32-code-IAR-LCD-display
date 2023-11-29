/*
 * Nokia_5110.h
 *  Created on: 2018年6月13日
 *  Last Modify on: 2018年8月6日
 *  Modify Details: 增加了收发缓冲区，实现了硬件隔离，实现了高速设备和慢速设备的缓冲
 *      Author: Limerrence
 */
#ifndef __NOKIA5110_H_
#define __NOKIA5110_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define ByteLengh 4             //定义printf函数传递参数的位数
#define Data 1
#define Command 0

#define SPI 0    //选择硬件spi或者IO口模拟spi，
                 //1表示选择硬件spi(若选择硬件spi模式则会占用UCA0spi中断，
                 //注意其他IO口或第二功能不能与之冲突)，0表示选择IO口模拟spi
#define UART_MODE 3     //选择3线spi模式或者4线spi模式
#define UART_FIFO 1     //选择是否使用缓冲区

//-------------------硬件接口--------------------//
#if (SPI==1)
#define DC_DIR P1DIR |=BIT6                             //DC select
#define DC_1 P1OUT |=BIT6                               //DC select
#define DC_0 P1OUT &=~BIT6                              //DC select
#if (UART_MODE==3)
#define CE_DIR P3DIR |=BIT2
#define CE_1 P3OUT |=BIT2
#define CE_0 P3OUT &=~BIT2
#endif
#define RST_DIR P6DIR |=BIT6
#define RST_1 P6OUT |=BIT6
#define RST_0 P6OUT &=~BIT6
#endif
#if (SPI==0)


//--------------------stm32f103c8t6模拟硬件接口----------------------//
//#define RST_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define RST_1   GPIO_SetBits(GPIOA, GPIO_Pin_0)           //复位信号关
#define RST_0   GPIO_ResetBits(GPIOA, GPIO_Pin_0)         //复位信号开
//#define CE_DIR  PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define CE_1    GPIO_SetBits(GPIOA, GPIO_Pin_1)           //片选关
#define CE_0    GPIO_ResetBits(GPIOA, GPIO_Pin_1)         //片选开
//#define DC_DIR  PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 //DC select
#define DC_1    GPIO_SetBits(GPIOA, GPIO_Pin_2)           //选择数据
#define DC_0    GPIO_ResetBits(GPIOA, GPIO_Pin_2)         //选择命令
//#define Din_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define Din_1   GPIO_SetBits(GPIOA, GPIO_Pin_3)           //数据输入1
#define Din_0   GPIO_ResetBits(GPIOA, GPIO_Pin_3)        //数据输入0
//#define Clk_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define Clk_1   GPIO_SetBits(GPIOA, GPIO_Pin_4)          //时钟1
#define Clk_0   GPIO_ResetBits(GPIOA, GPIO_Pin_4)        //时钟0

/*
//-------------------msp430f5529模拟硬件接口-----------------------//
#define RST_DIR P3DIR |=BIT0
#define RST_1 P3OUT |=BIT0      //复位信号关
#define RST_0 P3OUT &=~BIT0     //复位信号开
#define CE_DIR P3DIR |=BIT1
#define CE_1 P3OUT |=BIT1       //片选关
#define CE_0 P3OUT &=~BIT1      //片选开
#define DC_DIR P2DIR |=BIT6                             //DC select
#define DC_1 P2OUT |=BIT6       //选择数据
#define DC_0 P2OUT &=~BIT6      //选择命令
#define Din_DIR P2DIR |=BIT3
#define Din_1 P2OUT |=BIT3      //数据输入1
#define Din_0 P2OUT &=~BIT3     //数据输入0
#define Clk_DIR P8DIR |=BIT1
#define Clk_1 P8OUT |=BIT1      //时钟1
#define Clk_0 P8OUT &=~BIT1     //时钟0

*/

#endif
//-------------------------以下是函数声明-------------------------//
extern void spi_Init();
char LCD_Write_Byte(unsigned char data,unsigned char command);
char LCD_Write_Byte(unsigned char data,unsigned char command);
void LCD_Set_XY(unsigned char X, unsigned char Y);
void LCD_Write_Char(unsigned char c);
extern void LCD_Write_String(unsigned char X,unsigned char Y,unsigned char *s);
extern void LCD_Clear_All(void);
extern void LCD_Init();
extern void Display_Mark();
void LCD_Write_Num(unsigned char X,unsigned char Y,unsigned char num);
extern void LCD_WriteResult(unsigned char X,unsigned char Y,unsigned long Result);
extern unsigned int Adress_BUFF;
extern unsigned int Data_BUFF;
extern void printf(unsigned char *String,volatile unsigned long *Adress);
//extern void LED_Init(void);
extern void delay_us (vu32 nCount);



#endif