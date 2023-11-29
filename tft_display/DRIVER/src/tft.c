#include "tft.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"
#include "stm32f10x.h"

//管理LCD重要参数
_lcd_dev lcddev;
#if USE_SEND_BUFF
send_buff_typedef send_buff; //使用缓冲区
#endif

/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPIv_WriteData(u8 Data)
{
    unsigned char i = 0;
    for ( i = 8; i > 0; i --)
    {
        LCD_SCL_CLR;
        if ( Data & 0x80)
            LCD_SDA_SET; //输出数据
        else
            LCD_SDA_CLR;
        LCD_SCL_SET;
        Data <<= 1;
    }
}

/****************************************************************************
* 名    称：u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
* 功    能：STM32_硬件SPI读写一个字节数据底层函数
* 入口参数：SPIx,Byte
* 出口参数：返回总线收到的数据
* 说    明：STM32_硬件SPI读写一个字节数据底层函数
****************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 Byte)
{
    while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET); //等待发送区空
    SPIx->DR = Byte;	 	                          //发送一个byte
    while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET);//等待接收完一个byte
    return SPIx->DR;                               //返回收到的数据
}

#if USE_SEND_BUFF
/****************************************************************************
* 名    称：send_buff_init()
* 功    能：初始化缓冲区
* 入口参数：无
* 出口参数：0：失败，1：成功
* 说    明：无
****************************************************************************/
u8 send_buff_init()
{
    send_buff.first_falg = 1;
    send_buff.data_length = 0;
    send_buff.p_read = 0;
    send_buff.p_write = 0;
    return (1);
}

/****************************************************************************
* 名    称：write_buff(_Bool type, u8 data)
* 功    能：向缓冲区写入一字节数据
* 入口参数：type：该字节数据对应的类型，1：数据，0：命令
*           data：数据
* 出口参数：0：失败，1：成功
* 说    明：type的值只能为1或0
****************************************************************************/
u8 write_buff(_Bool type, u8 data)
{
    while (send_buff.data_length >= SEND_BUFF_SIZE);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
    /*写指针等于读指针，说明buff是空的，直接将数据发送出去*/
    if (send_buff.first_falg)
    {
        send_buff.first_falg = 0;
        LCD_CS_CLR;
        if (type)
        {
            LCD_RS_SET;
        }
        else
        {
            LCD_RS_CLR;
        }
        SPI2->DR = data;
        SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
        return (1);
    }
    else
    {
        send_buff.data_length ++;
        if (type)
        {
            send_buff.tpye[send_buff.p_write] = 1;
        }
        else
        {
            send_buff.tpye[send_buff.p_write] = 0;
        }
        send_buff.data[send_buff.p_write] = data;
        send_buff.p_write = ++ send_buff.p_write >= SEND_BUFF_SIZE ? 0 : send_buff.p_write;
        SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
        return (1);
    }
}

/****************************************************************************
* 名    称：SPI2_IRQHandler(void)
* 功    能：SPI2中断函数
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void SPI2_IRQHandler(void)
{
    if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY ) == RESET)
    {
        LCD_CS_SET;//发送完毕，片选拉高
        if (send_buff.data_length > 0)//缓冲区由有待发数据
        {
            send_buff.data_length --;
            LCD_CS_CLR;     //开始发送数据，先将片选拉低

            if (send_buff.tpye[send_buff.p_read])//判断是数据还是命令
            {
                LCD_RS_SET;
            }
            else
            {
                LCD_RS_CLR;
            }
            SPI2->DR = send_buff.data[send_buff.p_read];
            send_buff.p_read = ++ send_buff.p_read >= SEND_BUFF_SIZE ? 0 : send_buff.p_read;
        }
        else
        {
            send_buff.first_falg = 1; //缓冲区空，标志置位，下一个数据可以直接发送
            SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);//将中断关闭
        }
    }
}
#endif

/****************************************************************************
* 名    称：void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
* 功    能：设置SPI的速度
* 入口参数：SPIx,SpeedSet
* 出口参数：无
* 说    明：SpeedSet:1,高速;0,低速;
****************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx, u8 SpeedSet)
{
    SPIx->CR1 &= 0XFFC7;
    if (SpeedSet == 1)                         //高速
    {
        SPIx->CR1 |= SPI_BaudRatePrescaler_2;   //Fsck=Fpclk/2
    }
    else                                        //低速
    {
        SPIx->CR1 |= SPI_BaudRatePrescaler_32;  //Fsck=Fpclk/32
    }
    SPIx->CR1 |= 1 << 6;                         //SPI设备使能
}

/****************************************************************************
* 名    称：spi2_related_rcc_init()
* 功    能：初始化和spi2相关的src时钟
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void spi2_related_rcc_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
        /*SPI2配置选项*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* 名    称：spi2_related_gpio_init()
* 功    能：初始化和spi2相关的gpio
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void spi2_related_gpio_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*配置spi相关管脚*/
#if USE_HARDNSS           //使用硬件NSS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | LCD_CS;
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
#endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*配置LED、RS、RST控制管脚*/
#if USE_HARDNSS           //使用硬件NSS
#ifdef	LCD_RST
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS | LCD_RST;
#else
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS;
#endif
#else
#ifdef	LCD_RST
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS | LCD_CS | LCD_RST;
#else
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS | LCD_CS;
#endif
#endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：spi2_config()
* 功    能：初始化spi2
* 入口参数：无
* 出口参数：无
* 说    明：无
****************************************************************************/
void spi2_config()
{
    /*SPI初始化*/
    SPI_InitTypeDef  SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;      //设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;        //时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;       //数据捕获于第二个时钟沿
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    /*SPI_CRCPolynomial定义了用于CRC值计算的多项式*/
    SPI_InitStructure.SPI_CRCPolynomial = 7;

#if USE_HARDNSS//硬件片选信号或软件片选信号
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
#else
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#endif

#if SPI_HIGH_SPEED_MODE//高速模式或低速模式
    /*波特率预分频值为2*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
    /*波特率预分频值为32*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
#endif
    SPI_Init(SPI2, &SPI_InitStructure);
#if USE_HARDNSS
    SPI_SSOutputCmd(SPI2, ENABLE);
#endif
#if USE_SEND_BUFF
    send_buff_init();
    SPI2->CR1 |= (1 << 6); //使能BUSY_FLAG
#endif
}

/****************************************************************************
* 名    称：spi2_related_nvic_init()
* 功    能：初始化和spi2相关的nvic
* 入口参数：无
* 出口参数：无
* 说    明：无
****************************************************************************/
void spi2_related_nvic_init()
{
    NVIC_InitTypeDef NVIC_InitStructure; //定义外部中断结构体

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //设置中断组优先级0
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;            //PPP 外部中断线
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //设置从优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 名    称：SPI2_Init(void)
* 功    能：STM32_SPI2硬件配置初始化
* 入口参数：无
* 出口参数：无
* 说    明：STM32_SPI2硬件配置初始化
****************************************************************************/
void SPI2_Init(void)
{
    /*配置相关的rcc时钟*/
    spi2_related_rcc_init();
    /*配置对应的串口引脚*/
    spi2_related_gpio_init();
    /*SPI2配置*/
    spi2_config();

    spi2_related_nvic_init();
    SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_TXE);
    /*使能SPI2*/
    SPI_Cmd(SPI2, ENABLE);
}

/****************************************************************************
* 名    称：LCD_WR_REG(u16 data)
* 功    能：向液晶屏总线写入写16位指令
* 入口参数：Reg:待写入的指令值
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_WR_REG(u16 data)
{
#if USE_SEND_BUFF
    write_buff(0, data >> 8);
    write_buff(0, data);
#else
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_CLR;          //软件控制片选信号
#endif
    LCD_RS_CLR;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2, data);
#else
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_SET;          //软件控制片选信号
#endif
#endif
}

/****************************************************************************
* 名    称：LCD_WR_DATA(u8 data)
* 功    能：向液晶屏总线写入写8位数据
* 入口参数：Data:待写入的数据
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_WR_DATA(u8 data)
{
#if USE_SEND_BUFF
    write_buff(1, data);
#else
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_CLR;          //软件控制片选信号
#endif
    LCD_RS_SET;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2,data);
#else
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_SET;          //软件控制片选信号
#endif
#endif
}

/****************************************************************************
* 名    称：LCD_DrawPoint_16Bit
* 功    能：8位总线下如何写入一个16位数据
* 入口参数：(x,y):光标坐标
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_WR_DATA_16Bit(u16 data)
{
#if USE_SEND_BUFF
    write_buff(1, data >> 8);
    write_buff(1, data);
#else
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_CLR;          //软件控制片选信号
#endif
    LCD_RS_SET;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2, data >> 8);
    SPI_WriteByte(SPI2, data);
#else
    SPIv_WriteData(data >> 8);
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //硬件控制片选信号

#else
    LCD_CS_SET;          //软件控制片选信号
#endif
#endif
}

/****************************************************************************
* 名    称：LCD_WriteReg
* 功    能：写寄存器数据
* 入口参数：LCD_Reg:寄存器地址   LCD_RegValue:要写入的数据
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/****************************************************************************
* 名    称：LCD_WriteRAM_Prepare
* 功    能：开始写GRAM
*           在给液晶屏传送RGB数据前，应该发送写GRAM指令
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

/****************************************************************************
* 名    称：LCD_DrawPoint
* 功    能：在指定位置写入一个像素点数据
* 入口参数：(x,y):光标坐标
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x,y);            //设置光标位置
    LCD_WR_DATA_16Bit(POINT_COLOR);
}

/****************************************************************************
* 名    称：LCD_GPIOInit
* 功    能：液晶屏IO初始化，液晶初始化前要调用此函数
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);

#ifdef LCD_RST
    GPIO_InitStructure.GPIO_Pin = LCD_LED | LCD_RS \
        | LCD_CS | LCD_SCL | LCD_SDA | LCD_RST;
#else
    GPIO_InitStructure.GPIO_Pin = LCD_LED| LCD_RS| LCD_CS | LCD_SCL | LCD_SDA;
#endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：LCD_Reset
* 功    能：LCD复位函数，液晶初始化前要调用此函数
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_RESET(void)
{
#ifdef LCD_RST
    LCD_RST_CLR;
#endif
    LCD_WR_REG(0x01);
    delay_ms(1);
#ifdef LCD_RST
    LCD_RST_SET;
#endif
    LCD_WR_REG(0x01);
    delay_ms(1);
}

/****************************************************************************
* 名    称：LCD_Init
* 功    能：LCD初始化
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void LCD_Init(void)
{
#if USE_HARDWARE_SPI         //使用硬件SPI
    SPI2_Init();
#else
    LCD_GPIOInit();          //使用模拟SPI
#endif
    LCD_RESET();             //液晶屏复位
    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x11);        //Sleep exit
    delay_ms (1);

    LCD_WR_REG(0xB1);        //ST7735R Frame Rate
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);

    LCD_WR_REG(0xB3);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);

    LCD_WR_REG(0xB4);       //Column inversion
    LCD_WR_DATA(0x07);


    LCD_WR_REG(0xC0);       //ST7735R Power Sequence
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x84);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0xC5);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x2A);
    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0xEE);

    LCD_WR_REG(0xC5);      //VCOM
    LCD_WR_DATA(0x0E);

    LCD_WR_REG(0x36);     //MX, MY, RGB mode
    LCD_WR_DATA(0xC8);

    /*ST7735R Gamma Sequence*/
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1a);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x2f);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x1f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x10);

    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x2c);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x2e);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x3f);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x10);

    LCD_WR_REG(0x2a);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x7f);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x9f);

    LCD_WR_REG(0xF0);     //Enable test command
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xF6);     //Disable ram power save mode
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x3A);     //65k mode
    LCD_WR_DATA(0x05);
    LCD_WR_REG(0x29);     //Display on

    LCD_SetParam();       //设置LCD参数
    LCD_LED_SET;          //点亮背光
    //LCD_Clear(WHITE);
}

/****************************************************************************
* 名    称： LCD_Clear
* 功    能： LCD全屏填充清屏函数
* 入口参数： Color:要清屏的填充色
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_Clear(u16 Color)
{
    u16 i,j;
    LCD_SetWindows(0, 0, lcddev.width-1, lcddev.height-1);
    for (i = 0; i < lcddev.width; i ++)
    {
        for (j = 0; j < lcddev.height; j ++)
        {
            LCD_WR_DATA_16Bit(Color);	             //写入数据
        }
    }
}

/****************************************************************************
* 名    称： LCD_Clear
* 功    能： LCD全屏填充清屏函数
* 入口参数： Color:要清屏的填充色
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_Clear_Part(u8 x_start, u8 y_start, u8 x_end, u8 y_end, u16 Color)
{
    u16 i,j;
    LCD_SetWindows(x_start, y_start, x_end - 1, y_end - 1);
    for (i = 0; i < lcddev.width; i ++)
    {
        for (j = 0; j < lcddev.height; j ++)
        {
            LCD_WR_DATA_16Bit(Color);	             //写入数据
        }
    }
}

/****************************************************************************
* 名    称： LCD_SetWindows
* 功    能： 设置lcd显示窗口，在此区域写点数据自动换行
* 入口参数： xy起点和终点
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd)
{
#if USE_HORIZONTAL == 1	         //使用横屏
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar + 32);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd + 32);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar + 0);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd + 0);
#else
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar>>8);
    LCD_WR_DATA(0x00FF&xStar+0);
    LCD_WR_DATA(xEnd>>8);
    LCD_WR_DATA(0x00FF&xEnd+0);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar>>8);
    LCD_WR_DATA(0x00FF&yStar+32);
    LCD_WR_DATA(yEnd>>8);
    LCD_WR_DATA(0x00FF&yEnd+32);
#endif
    LCD_WriteRAM_Prepare();	//开始写入GRAM
}

/****************************************************************************
* 名    称： LCD_SetCursor
* 功    能： 设置光标位置
* 入口参数： xy坐标
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/****************************************************************************
* 名    称： LCD_SetParam
* 功    能： 设置LCD参数，方便进行横竖屏模式切换
* 入口参数： 无
* 出口参数： 无
* 说    明：
****************************************************************************/
void LCD_SetParam(void)
{
    lcddev.wramcmd = 0x2C;
#if USE_HORIZONTAL == 1	//使用横屏
    lcddev.dir = 1;//横屏
    lcddev.width = 128;
    lcddev.height = 128;
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    LCD_WriteReg(0x36, 0xA8);

#else//竖屏
    lcddev.dir = 0;
    lcddev.width = 128;
    lcddev.height = 128;
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    LCD_WriteReg(0x36, 0xC8);
#endif
}


