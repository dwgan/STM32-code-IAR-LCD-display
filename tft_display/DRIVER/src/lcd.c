#include "lcd.h"

void lcd_gpio_spi(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void write_lcd_data_spi(u8 data)
{
    u8 data_bit, trans_data;
    trans_data = data;
    LCD_A0_H;
    LCD_CS_L;
    for (data_bit = 0; data_bit < 8; data_bit ++)
    {
        LCD_SCK_L;
        if ((trans_data & 0x80) == 0x80)
            LCD_MDO_H;
        else
            LCD_MDO_L;
        LCD_SCK_H;
        trans_data <<= 1;
    }
    LCD_CS_H;
}
void write_lcd_cmd_spi(u8 cmd)
{
    u8 data_bit, trans_data;
    trans_data = cmd;
    LCD_A0_L;
    LCD_CS_L;
    for (data_bit = 0; data_bit < 8; data_bit ++)
    {
        LCD_SCK_L;
        if ((trans_data & 0x80) == 0x80)
            LCD_MDO_H;
        else
            LCD_MDO_L;
        LCD_SCK_H;
        trans_data <<= 1;
    }
    LCD_CS_H;
}