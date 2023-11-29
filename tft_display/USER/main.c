#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdarg.h"
#include "timer.h"
#include "gui.h"
#include "tft.h"
#include "dht11.h"
int main()
{
    SystemInit();                //��ʼ��ϵͳ��ϵͳʱ���趨Ϊ72MHz
    led_gpio_init();
    delay_init();               //����systick���ж�ʱ������Ϊ72000/72000000 = 1us
    LCD_Init();	                //Һ������ʼ��
    delay_ms(10);
    LCD_Clear(WHITE);
    u16 temp, hum;
//    my_printf("\nHello World!\nI am %d\nGood Luck!", 666);
    while(1)
    {
        if (dht11_read_data())
        {
            temp = Data_Buff[2];
            hum = Data_Buff[0];
        }
        LCD_Clear_Part(48, 16, 64, 48, WHITE);
        my_printf("\n�¶�: %d ��", temp);
        my_printf("\n\nʪ��: %d %RH", hum);
        delay_ms(1000);
    }
}
