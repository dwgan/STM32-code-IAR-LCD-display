#include "timer.h"

__IO uint32_t TimingDelay;

void systick_init(__IO uint32_t systick)
{
    /*����systick����ֵ��ϵͳʱ��Ϊ72MHz*/
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

void sys_delay_us(__IO uint32_t nTime)//�ӳٺ���������ΪUS
{
    TimingDelay = nTime;              //ʱ�ӵδ���
    while(TimingDelay != 0);
}

void sys_delay_ms(__IO uint32_t nTime)//�ӳٺ���������ΪUS
{
    TimingDelay = 1000*nTime;              //ʱ�ӵδ���
    while(TimingDelay != 0);
}