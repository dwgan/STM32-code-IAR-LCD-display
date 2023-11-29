/*
 * Nokia_5110.h
 *  Created on: 2018��6��13��
 *  Last Modify on: 2018��8��6��
 *  Modify Details: �������շ���������ʵ����Ӳ�����룬ʵ���˸����豸�������豸�Ļ���
 *      Author: Limerrence
 */
#ifndef __NOKIA5110_H_
#define __NOKIA5110_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define ByteLengh 4             //����printf�������ݲ�����λ��
#define Data 1
#define Command 0

#define SPI 0    //ѡ��Ӳ��spi����IO��ģ��spi��
                 //1��ʾѡ��Ӳ��spi(��ѡ��Ӳ��spiģʽ���ռ��UCA0spi�жϣ�
                 //ע������IO�ڻ�ڶ����ܲ�����֮��ͻ)��0��ʾѡ��IO��ģ��spi
#define UART_MODE 3     //ѡ��3��spiģʽ����4��spiģʽ
#define UART_FIFO 1     //ѡ���Ƿ�ʹ�û�����

//-------------------Ӳ���ӿ�--------------------//
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


//--------------------stm32f103c8t6ģ��Ӳ���ӿ�----------------------//
//#define RST_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define RST_1   GPIO_SetBits(GPIOA, GPIO_Pin_0)           //��λ�źŹ�
#define RST_0   GPIO_ResetBits(GPIOA, GPIO_Pin_0)         //��λ�źſ�
//#define CE_DIR  PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define CE_1    GPIO_SetBits(GPIOA, GPIO_Pin_1)           //Ƭѡ��
#define CE_0    GPIO_ResetBits(GPIOA, GPIO_Pin_1)         //Ƭѡ��
//#define DC_DIR  PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 //DC select
#define DC_1    GPIO_SetBits(GPIOA, GPIO_Pin_2)           //ѡ������
#define DC_0    GPIO_ResetBits(GPIOA, GPIO_Pin_2)         //ѡ������
//#define Din_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define Din_1   GPIO_SetBits(GPIOA, GPIO_Pin_3)           //��������1
#define Din_0   GPIO_ResetBits(GPIOA, GPIO_Pin_3)        //��������0
//#define Clk_DIR PGPIO_InitStructure.GPIO_Pin = GPIO_Pin_13
#define Clk_1   GPIO_SetBits(GPIOA, GPIO_Pin_4)          //ʱ��1
#define Clk_0   GPIO_ResetBits(GPIOA, GPIO_Pin_4)        //ʱ��0

/*
//-------------------msp430f5529ģ��Ӳ���ӿ�-----------------------//
#define RST_DIR P3DIR |=BIT0
#define RST_1 P3OUT |=BIT0      //��λ�źŹ�
#define RST_0 P3OUT &=~BIT0     //��λ�źſ�
#define CE_DIR P3DIR |=BIT1
#define CE_1 P3OUT |=BIT1       //Ƭѡ��
#define CE_0 P3OUT &=~BIT1      //Ƭѡ��
#define DC_DIR P2DIR |=BIT6                             //DC select
#define DC_1 P2OUT |=BIT6       //ѡ������
#define DC_0 P2OUT &=~BIT6      //ѡ������
#define Din_DIR P2DIR |=BIT3
#define Din_1 P2OUT |=BIT3      //��������1
#define Din_0 P2OUT &=~BIT3     //��������0
#define Clk_DIR P8DIR |=BIT1
#define Clk_1 P8OUT |=BIT1      //ʱ��1
#define Clk_0 P8OUT &=~BIT1     //ʱ��0

*/

#endif
//-------------------------�����Ǻ�������-------------------------//
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