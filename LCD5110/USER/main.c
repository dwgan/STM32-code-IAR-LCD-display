#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "nokia5110.h"
//#include "stm32f10x_lib.h"//头文件
int main()
{
    LCD_Init();
    unsigned long d = 33;
    while(1)
    {   
        d++;
        printf("a=100\nb=50\nc=a-b\n\nc2=%d",&d);
        GPIO_SetBits(GPIOC, GPIO_Pin_13);//将 GPIO的A端口第二个管脚置为高电平，即PA.1=1;
        delay_us(0xffff);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);//将 GPIO的 A端口第二个管脚置为低电平，即PA.1=0;
        delay_us(0xffff);
    }
}