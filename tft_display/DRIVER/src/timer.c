#include "timer.h"

__IO uint32_t TimingDelay;

void systick_init(__IO uint32_t systick)
{
    /*配置systick重载值，系统时钟为72MHz*/
    while (SysTick_Config(systick));
}

void TimerDelay_Decrement(void)
{
    if(TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}


void SysTick_Handler(void)
{
    TimerDelay_Decrement();
}

void sys_delay_us(__IO uint32_t nTime)//延迟函数，设置为US
{
    TimingDelay = nTime;              //时钟滴答数
    while(TimingDelay != 0);
}

void sys_delay_ms(__IO uint32_t nTime)//延迟函数，设置为US
{
    TimingDelay = 1000*nTime;              //时钟滴答数
    while(TimingDelay != 0);
}