#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "nokia5110.h"
#include "timer.h"
int main()
{
    SystemInit();                         //��ʼ��ϵͳ��ϵͳʱ���趨Ϊ72MHz
    systick_init(72);                  //����systick���ж�ʱ������Ϊ72000/72000000 = 1ms
    LCD_Init();
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE); //�� GPIOA ʱ��
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //GPIO�Ĺ���״̬Ϊ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO���������������� 50Hz
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA
    unsigned long d = 0;
    while(1)
    {   
        d++;
        printf("\n\n Hello World!\n\n     %d",&d);
        GPIO_SetBits(GPIOA, GPIO_Pin_5); //�� GPIO��A�˿ڵڶ����ܽ���Ϊ�ߵ�ƽ����PA.1=1;
        delay_ms(100);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);//�� GPIO�� A�˿ڵڶ����ܽ���Ϊ�͵�ƽ����PA.1=0;
        delay_ms(100);
    }
}