#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "nokia5110.h"
#include "timer.h"
int main()
{
    SystemInit();                         //初始化系统，系统时钟设定为72MHz
    systick_init(72);                  //配置systick，中断时间设置为72000/72000000 = 1ms
    LCD_Init();
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE); //开 GPIOA 时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //GPIO的工作状态为推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO的速率最高输出速率 50Hz
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA
    unsigned long d = 0;
    while(1)
    {   
        d++;
        printf("\n\n Hello World!\n\n     %d",&d);
        GPIO_SetBits(GPIOA, GPIO_Pin_5); //将 GPIO的A端口第二个管脚置为高电平，即PA.1=1;
        delay_ms(100);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);//将 GPIO的 A端口第二个管脚置为低电平，即PA.1=0;
        delay_ms(100);
    }
}