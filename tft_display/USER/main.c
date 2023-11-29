#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdarg.h"
#include "timer.h"
#include "gui.h"
#include "tft.h"
#include "dht11.h"
int main()
{
    SystemInit();                //初始化系统，系统时钟设定为72MHz
    led_gpio_init();
    delay_init();               //配置systick，中断时间设置为72000/72000000 = 1us
    LCD_Init();	                //液晶屏初始化
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
        my_printf("\n温度: %d ℃", temp);
        my_printf("\n\n湿度: %d %RH", hum);
        delay_ms(1000);
    }
}
