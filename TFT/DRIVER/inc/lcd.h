#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"

// STM32接口------->TFT接口
// PB0 				连接至TFT -LED
// PB1					连接至TFT --RS
// PB12 				连接至TFT --CS
// RST(也可以IO控制)	TFT--RST
// PB13				连接至TFT -- CLK
// PB15				连接至TFT - SDI


// 可以直接配合本店的STM32F103R最小系统使用，带有TFT接口：
// https://item.taobao.com/item.htm?id=523336310868

// TFT1.44液晶屏购买链接：
// https://item.taobao.com/item.htm?id=522808579863


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换，支持8/16位模式切换
#define USE_HORIZONTAL  	0	//定义是否使用横屏 		0,不使用.1,使用.
//使用硬件SPI 还是模拟SPI作为测试
#define USE_HARDWARE_SPI    0  //1:Enable Hardware SPI;0:USE Soft SPI
//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//该模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define LCD_LED        	GPIO_Pin_0  //PB0 连接至TFT -LED
#define LCD_CTRL   	  	GPIOB		//定义TFT数据端口
#define LCD_RS         	GPIO_Pin_1	//PB1连接至TFT --RS
#define LCD_CS        	GPIO_Pin_12 //PB12 连接至TFT --CS
// #define LCD_RST     	GPIO_Pin_10	//PB10连接至TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13连接至TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15连接至TFT - SDI
							    
//////////////////////////////////////////////////////////////////////
//液晶控制口置1操作语句宏定义
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL    
#ifdef LCD_RST
#define	LCD_RST_SET  	LCD_CTRL->BSRR=LCD_RST 
#endif 
#define	LCD_LED_SET  	LCD_CTRL->BSRR=LCD_LED   

//液晶控制口置0操作语句宏定义
#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	LCD_CTRL->BRR=LCD_RS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    
#ifdef LCD_RST
#define	LCD_RST_CLR  	LCD_CTRL->BRR=LCD_RST  
#endif  
#define	LCD_LED_CLR  	LCD_CTRL->BRR=LCD_LED 

//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
//#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
extern u16 BACK_COLOR, POINT_COLOR ;  

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//画点
u16  LCD_ReadPoint(u16 x,u16 y); //读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_DrawPoint_16Bit(u16 color);
u16 LCD_RD_DATA(void);//读取LCD数据									    
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
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void  SPIv_WriteData(u8 Data);
void LCD_WR_REG(u16 data);

						  		 
#endif  
	 
	 



