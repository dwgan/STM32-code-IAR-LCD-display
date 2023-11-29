#include "key.h"

key_state key0, key1;

void key_gpio_config (void)
{
    //����GPIOB ��AFIO�����ö˿ڣ�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure; //����GPIO �ṹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //��ʼ��GPIO

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); //��PB11 ��EXIT_11 ����
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //��PB11 ��EXIT_11 ����

    /*ѡ��EXTI_Line11*/
    EXTI_InitTypeDef EXTI_InitStructure; //�����жϿ������ṹ��

    EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ⲿ�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��س���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure; //�����ⲿ�жϽṹ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //�����ж������ȼ�0
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;      //PPP �ⲿ�ж���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //���ô����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
    u16 i, j;
    if(EXTI_GetITStatus(EXTI_Line11) == SET) //�����EXIT_11 �������жϣ�����
    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //�ӳ�һ�ᣬ10ms
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == RESET) //����
        {
            key0.led_on_off = key0.led_on_off >= 3 ? 0 : key0.led_on_off + 1;
            key0.key_change_bit = CHGE_IN;
        }
        EXTI_ClearITPendingBit(EXTI_Line12); //�����־
    }
    if(EXTI_GetITStatus(EXTI_Line14) == SET) //�����EXIT_11 �������жϣ�����
    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //�ӳ�һ�ᣬ10ms
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == RESET) //����
        {
            key1.led_on_off = key1.led_on_off >= 3 ? 0 : key1.led_on_off + 1;
            key1.key_change_bit = CHGE_IN;
        }
        EXTI_ClearITPendingBit(EXTI_Line13); //�����־
    }
//    if(EXTI_GetITStatus(EXTI_Line14) == SET) //�����EXIT_11 �������жϣ�����
//    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //�ӳ�һ�ᣬ10ms
//        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET) //����
//        {
//            key2.led_on_off = key2.led_on_off >= 3 ? 0 : key2.led_on_off + 1;
//            key2.key_change_bit = CHGE_IN;
//        }
//        EXTI_ClearITPendingBit(EXTI_Line14); //�����־
//    }
//    if(EXTI_GetITStatus(EXTI_Line15) == SET) //�����EXIT_11 �������жϣ�����
//    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //�ӳ�һ�ᣬ10ms
//        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == RESET) //����
//        {
//            key3.led_on_off = key3.led_on_off >= 3 ? 0 : key3.led_on_off + 1;
//            key3.key_change_bit = CHGE_IN;
//        }
//        EXTI_ClearITPendingBit(EXTI_Line15); //�����־
//    }
}