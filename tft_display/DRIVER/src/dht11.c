#include "dht11.h"
u16 Data_Buff[5];

void dht11_portin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //Configure pin as input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void dht11_portout(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //Configure pin as input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void dht11_reset(void)
{
    dht11_portout();
    DHT11_OUT_L;
    delay_us(19000);
    DHT11_OUT_H;
    delay_us(25);
    dht11_portin();
}

u16 dht11_scan(void)
{
    return DHT11_IN;
}

u16 dht11_read_bit(void)
{
    while(DHT11_IN == RESET);
    delay_us(40);
    if(DHT11_IN == SET)
    {
        while(DHT11_IN == SET);
        return 1;
    }
    else
        return 0;
}

u16 dht11_read_byte(void)
{
    u16 i;
    u16 data = 0;
    for(i=0;i<8;i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

u16 dht11_read_data(void)
{
    u16 i = 0;
    dht11_reset();
    //����⵽DHT11 ��STM32 ����оƬ������ѹΪ�͵�ѹʱ��˵��DHT11 ��ʼ��Ӧ
    if (dht11_scan() == 0)
    {
        while(DHT11_IN == RESET); //�ȴ�80us �ĵ͵�ƽ��Ӧ�źŽ���
        while(DHT11_IN == SET); //�ȴ�80us �ĸߵ�ƽ��������ʼ��������
        for (i = 0; i < 5; i++)
        {
            Data_Buff[i] = dht11_read_byte();
        }
        while(DHT11_IN == RESET); //�ȴ�40 λ�����������50us �͵�ƽ�������
        dht11_portout();
        DHT11_OUT_H;
        if ((Data_Buff[0]+Data_Buff[1]+Data_Buff[2]+Data_Buff[3]) == Data_Buff[4])
        {
            return 1;
        }
        else //У�����
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


