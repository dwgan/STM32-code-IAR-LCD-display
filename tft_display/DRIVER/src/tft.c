#include "tft.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"
#include "stm32f10x.h"

//����LCD��Ҫ����
_lcd_dev lcddev;
#if USE_SEND_BUFF
send_buff_typedef send_buff; //ʹ�û�����
#endif

/****************************************************************************
* ��    �ƣ�void  SPIv_WriteData(u8 Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void  SPIv_WriteData(u8 Data)
{
    unsigned char i = 0;
    for ( i = 8; i > 0; i --)
    {
        LCD_SCL_CLR;
        if ( Data & 0x80)
            LCD_SDA_SET; //�������
        else
            LCD_SDA_CLR;
        LCD_SCL_SET;
        Data <<= 1;
    }
}

/****************************************************************************
* ��    �ƣ�u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
* ��    �ܣ�STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
* ��ڲ�����SPIx,Byte
* ���ڲ��������������յ�������
* ˵    ����STM32_Ӳ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 Byte)
{
    while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET); //�ȴ���������
    SPIx->DR = Byte;	 	                          //����һ��byte
    while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�������һ��byte
    return SPIx->DR;                               //�����յ�������
}

#if USE_SEND_BUFF
/****************************************************************************
* ��    �ƣ�send_buff_init()
* ��    �ܣ���ʼ��������
* ��ڲ�������
* ���ڲ�����0��ʧ�ܣ�1���ɹ�
* ˵    ������
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
* ��    �ƣ�write_buff(_Bool type, u8 data)
* ��    �ܣ��򻺳���д��һ�ֽ�����
* ��ڲ�����type�����ֽ����ݶ�Ӧ�����ͣ�1�����ݣ�0������
*           data������
* ���ڲ�����0��ʧ�ܣ�1���ɹ�
* ˵    ����type��ֵֻ��Ϊ1��0
****************************************************************************/
u8 write_buff(_Bool type, u8 data)
{
    while (send_buff.data_length >= SEND_BUFF_SIZE);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
    /*дָ����ڶ�ָ�룬˵��buff�ǿյģ�ֱ�ӽ����ݷ��ͳ�ȥ*/
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
* ��    �ƣ�SPI2_IRQHandler(void)
* ��    �ܣ�SPI2�жϺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void SPI2_IRQHandler(void)
{
    if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY ) == RESET)
    {
        LCD_CS_SET;//������ϣ�Ƭѡ����
        if (send_buff.data_length > 0)//���������д�������
        {
            send_buff.data_length --;
            LCD_CS_CLR;     //��ʼ�������ݣ��Ƚ�Ƭѡ����

            if (send_buff.tpye[send_buff.p_read])//�ж������ݻ�������
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
            send_buff.first_falg = 1; //�������գ���־��λ����һ�����ݿ���ֱ�ӷ���
            SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);//���жϹر�
        }
    }
}
#endif

/****************************************************************************
* ��    �ƣ�void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
* ��    �ܣ�����SPI���ٶ�
* ��ڲ�����SPIx,SpeedSet
* ���ڲ�������
* ˵    ����SpeedSet:1,����;0,����;
****************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx, u8 SpeedSet)
{
    SPIx->CR1 &= 0XFFC7;
    if (SpeedSet == 1)                         //����
    {
        SPIx->CR1 |= SPI_BaudRatePrescaler_2;   //Fsck=Fpclk/2
    }
    else                                        //����
    {
        SPIx->CR1 |= SPI_BaudRatePrescaler_32;  //Fsck=Fpclk/32
    }
    SPIx->CR1 |= 1 << 6;                         //SPI�豸ʹ��
}

/****************************************************************************
* ��    �ƣ�spi2_related_rcc_init()
* ��    �ܣ���ʼ����spi2��ص�srcʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void spi2_related_rcc_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
        /*SPI2����ѡ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* ��    �ƣ�spi2_related_gpio_init()
* ��    �ܣ���ʼ����spi2��ص�gpio
* ��ڲ�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void spi2_related_gpio_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����spi��عܽ�*/
#if USE_HARDNSS           //ʹ��Ӳ��NSS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | LCD_CS;
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
#endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*����LED��RS��RST���ƹܽ�*/
#if USE_HARDNSS           //ʹ��Ӳ��NSS
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
* ��    �ƣ�spi2_config()
* ��    �ܣ���ʼ��spi2
* ��ڲ�������
* ���ڲ�������
* ˵    ������
****************************************************************************/
void spi2_config()
{
    /*SPI��ʼ��*/
    SPI_InitTypeDef  SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;      //����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;        //ʱ�����ո�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;       //���ݲ����ڵڶ���ʱ����
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    /*SPI_CRCPolynomial����������CRCֵ����Ķ���ʽ*/
    SPI_InitStructure.SPI_CRCPolynomial = 7;

#if USE_HARDNSS//Ӳ��Ƭѡ�źŻ����Ƭѡ�ź�
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
#else
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#endif

#if SPI_HIGH_SPEED_MODE//����ģʽ�����ģʽ
    /*������Ԥ��ƵֵΪ2*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
    /*������Ԥ��ƵֵΪ32*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
#endif
    SPI_Init(SPI2, &SPI_InitStructure);
#if USE_HARDNSS
    SPI_SSOutputCmd(SPI2, ENABLE);
#endif
#if USE_SEND_BUFF
    send_buff_init();
    SPI2->CR1 |= (1 << 6); //ʹ��BUSY_FLAG
#endif
}

/****************************************************************************
* ��    �ƣ�spi2_related_nvic_init()
* ��    �ܣ���ʼ����spi2��ص�nvic
* ��ڲ�������
* ���ڲ�������
* ˵    ������
****************************************************************************/
void spi2_related_nvic_init()
{
    NVIC_InitTypeDef NVIC_InitStructure; //�����ⲿ�жϽṹ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //�����ж������ȼ�0
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;            //PPP �ⲿ�ж���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //���ô����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ��    �ƣ�SPI2_Init(void)
* ��    �ܣ�STM32_SPI2Ӳ�����ó�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����STM32_SPI2Ӳ�����ó�ʼ��
****************************************************************************/
void SPI2_Init(void)
{
    /*������ص�rccʱ��*/
    spi2_related_rcc_init();
    /*���ö�Ӧ�Ĵ�������*/
    spi2_related_gpio_init();
    /*SPI2����*/
    spi2_config();

    spi2_related_nvic_init();
    SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_TXE);
    /*ʹ��SPI2*/
    SPI_Cmd(SPI2, ENABLE);
}

/****************************************************************************
* ��    �ƣ�LCD_WR_REG(u16 data)
* ��    �ܣ���Һ��������д��д16λָ��
* ��ڲ�����Reg:��д���ָ��ֵ
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_WR_REG(u16 data)
{
#if USE_SEND_BUFF
    write_buff(0, data >> 8);
    write_buff(0, data);
#else
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_CLR;          //�������Ƭѡ�ź�
#endif
    LCD_RS_CLR;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2, data);
#else
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_SET;          //�������Ƭѡ�ź�
#endif
#endif
}

/****************************************************************************
* ��    �ƣ�LCD_WR_DATA(u8 data)
* ��    �ܣ���Һ��������д��д8λ����
* ��ڲ�����Data:��д�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_WR_DATA(u8 data)
{
#if USE_SEND_BUFF
    write_buff(1, data);
#else
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_CLR;          //�������Ƭѡ�ź�
#endif
    LCD_RS_SET;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2,data);
#else
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_SET;          //�������Ƭѡ�ź�
#endif
#endif
}

/****************************************************************************
* ��    �ƣ�LCD_DrawPoint_16Bit
* ��    �ܣ�8λ���������д��һ��16λ����
* ��ڲ�����(x,y):�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_WR_DATA_16Bit(u16 data)
{
#if USE_SEND_BUFF
    write_buff(1, data >> 8);
    write_buff(1, data);
#else
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_CLR;          //�������Ƭѡ�ź�
#endif
    LCD_RS_SET;
#if USE_HARDWARE_SPI
    SPI_WriteByte(SPI2, data >> 8);
    SPI_WriteByte(SPI2, data);
#else
    SPIv_WriteData(data >> 8);
    SPIv_WriteData(data);
#endif
#if USE_HARDNSS          //Ӳ������Ƭѡ�ź�

#else
    LCD_CS_SET;          //�������Ƭѡ�ź�
#endif
#endif
}

/****************************************************************************
* ��    �ƣ�LCD_WriteReg
* ��    �ܣ�д�Ĵ�������
* ��ڲ�����LCD_Reg:�Ĵ�����ַ   LCD_RegValue:Ҫд�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/****************************************************************************
* ��    �ƣ�LCD_WriteRAM_Prepare
* ��    �ܣ���ʼдGRAM
*           �ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

/****************************************************************************
* ��    �ƣ�LCD_DrawPoint
* ��    �ܣ���ָ��λ��д��һ�����ص�����
* ��ڲ�����(x,y):�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x,y);            //���ù��λ��
    LCD_WR_DATA_16Bit(POINT_COLOR);
}

/****************************************************************************
* ��    �ƣ�LCD_GPIOInit
* ��    �ܣ�Һ����IO��ʼ����Һ����ʼ��ǰҪ���ô˺���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
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
* ��    �ƣ�LCD_Reset
* ��    �ܣ�LCD��λ������Һ����ʼ��ǰҪ���ô˺���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
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
* ��    �ƣ�LCD_Init
* ��    �ܣ�LCD��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
****************************************************************************/
void LCD_Init(void)
{
#if USE_HARDWARE_SPI         //ʹ��Ӳ��SPI
    SPI2_Init();
#else
    LCD_GPIOInit();          //ʹ��ģ��SPI
#endif
    LCD_RESET();             //Һ������λ
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

    LCD_SetParam();       //����LCD����
    LCD_LED_SET;          //��������
    //LCD_Clear(WHITE);
}

/****************************************************************************
* ��    �ƣ� LCD_Clear
* ��    �ܣ� LCDȫ�������������
* ��ڲ����� Color:Ҫ���������ɫ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_Clear(u16 Color)
{
    u16 i,j;
    LCD_SetWindows(0, 0, lcddev.width-1, lcddev.height-1);
    for (i = 0; i < lcddev.width; i ++)
    {
        for (j = 0; j < lcddev.height; j ++)
        {
            LCD_WR_DATA_16Bit(Color);	             //д������
        }
    }
}

/****************************************************************************
* ��    �ƣ� LCD_Clear
* ��    �ܣ� LCDȫ�������������
* ��ڲ����� Color:Ҫ���������ɫ
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_Clear_Part(u8 x_start, u8 y_start, u8 x_end, u8 y_end, u16 Color)
{
    u16 i,j;
    LCD_SetWindows(x_start, y_start, x_end - 1, y_end - 1);
    for (i = 0; i < lcddev.width; i ++)
    {
        for (j = 0; j < lcddev.height; j ++)
        {
            LCD_WR_DATA_16Bit(Color);	             //д������
        }
    }
}

/****************************************************************************
* ��    �ƣ� LCD_SetWindows
* ��    �ܣ� ����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
* ��ڲ����� xy�����յ�
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd)
{
#if USE_HORIZONTAL == 1	         //ʹ�ú���
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
    LCD_WriteRAM_Prepare();	//��ʼд��GRAM
}

/****************************************************************************
* ��    �ƣ� LCD_SetCursor
* ��    �ܣ� ���ù��λ��
* ��ڲ����� xy����
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/****************************************************************************
* ��    �ƣ� LCD_SetParam
* ��    �ܣ� ����LCD������������к�����ģʽ�л�
* ��ڲ����� ��
* ���ڲ����� ��
* ˵    ����
****************************************************************************/
void LCD_SetParam(void)
{
    lcddev.wramcmd = 0x2C;
#if USE_HORIZONTAL == 1	//ʹ�ú���
    lcddev.dir = 1;//����
    lcddev.width = 128;
    lcddev.height = 128;
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    LCD_WriteReg(0x36, 0xA8);

#else//����
    lcddev.dir = 0;
    lcddev.width = 128;
    lcddev.height = 128;
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    LCD_WriteReg(0x36, 0xC8);
#endif
}


