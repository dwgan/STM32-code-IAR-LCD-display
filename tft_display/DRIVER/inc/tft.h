#ifndef __TFT_H
#define __TFT_H
#include "system.h"
#include "stdlib.h"

#include "gpio_config.h"
/***********************************�û�������***************************************/
//֧�ֺ��������ٶ����л���֧��8/16λģʽ�л�
#define USE_HORIZONTAL  1    //�����Ƿ�ʹ�ú��� 0����ʹ�� 1��ʹ��.
#define USE_HARDWARE_SPI 1    //1:Enable Hardware SPI 0:USE Soft SPI
#define USE_HARDNSS  0        //ʹ��Ӳ��NSS
#define SPI_HIGH_SPEED_MODE 1 //SPI����ģʽ  1������ģʽ 0������ģʽ
#define USE_SEND_BUFF 0       //�����Ƿ�ʹ�÷��ͻ�����
#define USE_RECEIVE_BUFF 0    //�����Ƿ�ʹ�ý��ջ�����
/*********************************�������û�������***********************************/

/***********************************�ӿڶ���******************************************
* STM32�ӿ�------------------>TFT�ӿ�
* PB0                         ������TFT -LED
* PB1                         ������TFT --RS
* PB12                        ������TFT --CS
* RST(Ҳ����IO����)	    TFT--RST
* PB13	                     ������TFT -- CLK
* PB15	                     ������TFT - SDI
**********************************�����ǽӿڶ���*************************************/

/***********************************��ɫ����******************************************/
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN       0XBC40 //��ɫ
#define BRRED       0XFC07 //�غ�ɫ
#define GRAY        0X8430 //��ɫ

//������ɫΪPANEL����ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ
#define GRAYBLUE       	 0X5458 //����ɫ


#define LIGHTGREEN        0X841F //ǳ��ɫ
//#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY             0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	  0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE            0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

/***********************************��������ɫ����***********************************/

/*LCD��Ҫ������*/
typedef struct
{
	u16 width;		//LCD ���
	u16 height;		//LCD �߶�
	u16 id;			//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������
	u16 wramcmd;	        //��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ��
}_lcd_dev;

extern _lcd_dev lcddev;	//����LCD��Ҫ����

//����LCD�ĳߴ�
#if USE_HORIZONTAL == 1	   //ʹ�ú���
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif
/**********************************����������*********************************/
#define SEND_BUFF_SIZE 9150
#define RECEIVE_BUFF_SIZE 6000
#if USE_SEND_BUFF
typedef struct
{
    _Bool first_falg;
    u32 data_length;
    u32 p_write;
    u32 p_read;
    _Bool tpye[SEND_BUFF_SIZE];
    u8 data[SEND_BUFF_SIZE];
}send_buff_typedef;
extern send_buff_typedef send_buff;
#endif

#if USE_RECEIVE_BUFF
typedef struct
{
    u32 pointer;
    _Bool tpye[RECEIVE_BUFF_SIZE];
    u8 data[RECEIVE_BUFF_SIZE];
}receive_buff_tpyedef;
extern receive_buff_tpyedef receive_buff;
#endif
/******************************�����ǻ���������*******************************/



/***********************************printf��������************************************/
#define START_X 0           //printf�����Ŀ�ʼ������
#define START_Y 0           //printf�����Ŀ�ʼ������
#define PRINTF_FC RED      //printf������������ɫ
#define PRINTF_BC YELLOW     //printf�����ı�����ɫ
#define PRINTF_SIZE 16      //printf�����������С��16��16x8/16x16
#define PRINTF_MODE 1       //printf��������ʾģʽ��1�����ӣ�0��������
#define NUM_LENGTH 5        //��ʾ���ֵ�λ��
/**********************************������printf��������*******************************/




/************************************LCD�˿ڶ���*************************************/
//��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define LCD_LED        	GPIO_Pin_0      //PB0 ������TFT -LED
#define LCD_CTRL        	GPIOB		//����TFT���ݶ˿�
#define LCD_RS         	GPIO_Pin_1	//PB1������TFT --RS
#define LCD_CS        	GPIO_Pin_12     //PB12 ������TFT --CS
#define LCD_RST      	GPIO_Pin_10	//PB10������TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13������TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15������TFT - SDI

//Һ�����ƿ���1�������궨��
#define	LCD_CS_SET      	LCD_CTRL->BSRR=LCD_CS
#define	LCD_RS_SET      	LCD_CTRL->BSRR=LCD_RS
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL
#ifdef LCD_RST
#define	LCD_RST_SET  	LCD_CTRL->BSRR=LCD_RST
#endif
#define	LCD_LED_SET  	LCD_CTRL->BSRR=LCD_LED

//Һ�����ƿ���0�������궨��
#define	LCD_CS_CLR      	LCD_CTRL->BRR=LCD_CS
#define	LCD_RS_CLR      	LCD_CTRL->BRR=LCD_RS
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL
#ifdef LCD_RST
#define	LCD_RST_CLR  	LCD_CTRL->BRR=LCD_RST
#endif
#define	LCD_LED_CLR  	LCD_CTRL->BRR=LCD_LED

extern u32 buff_count;
extern u32 buff_write;

extern u16 BACK_COLOR, POINT_COLOR;

//TFTLCD������Ҫ���õĺ���
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);
void LCD_Clear_Part(u8 x_start, u8 y_start, u8 x_end, u8 y_end, u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x, u16 y);//����
u16  LCD_ReadPoint(u16 x, u16 y); //����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd);
void LCD_DrawPoint_16Bit(u16 color);
u16 LCD_RD_DATA(void);//��ȡLCD����
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
void LCD_WR_DATA_16Bit(u16 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);

void SPI2_Init(void);
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 Byte);
void  SPIv_WriteData(u8 Data);
void LCD_WR_REG(u16 data);


#endif





