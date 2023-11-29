#include "key.h"

key_state key0, key1;

void key_gpio_config (void)
{
    //开启GPIOB 和AFIO（复用端口）时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO 结构体

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //初始化GPIO

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); //将PB11 和EXIT_11 连接
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14); //将PB11 和EXIT_11 连接

    /*选择EXTI_Line11*/
    EXTI_InitTypeDef EXTI_InitStructure; //定义中断控制器结构体

    EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //外部中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿出发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure; //定义外部中断结构体

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //设置中断组优先级0
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;      //PPP 外部中断线
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //设置从优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
    u16 i, j;
    if(EXTI_GetITStatus(EXTI_Line11) == SET) //如果是EXIT_11 出发的中断，则处理
    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //延迟一会，10ms
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == RESET) //消抖
        {
            key0.led_on_off = key0.led_on_off >= 3 ? 0 : key0.led_on_off + 1;
            key0.key_change_bit = CHGE_IN;
        }
        EXTI_ClearITPendingBit(EXTI_Line12); //清除标志
    }
    if(EXTI_GetITStatus(EXTI_Line14) == SET) //如果是EXIT_11 出发的中断，则处理
    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //延迟一会，10ms
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == RESET) //消抖
        {
            key1.led_on_off = key1.led_on_off >= 3 ? 0 : key1.led_on_off + 1;
            key1.key_change_bit = CHGE_IN;
        }
        EXTI_ClearITPendingBit(EXTI_Line13); //清除标志
    }
//    if(EXTI_GetITStatus(EXTI_Line14) == SET) //如果是EXIT_11 出发的中断，则处理
//    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //延迟一会，10ms
//        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET) //消抖
//        {
//            key2.led_on_off = key2.led_on_off >= 3 ? 0 : key2.led_on_off + 1;
//            key2.key_change_bit = CHGE_IN;
//        }
//        EXTI_ClearITPendingBit(EXTI_Line14); //清除标志
//    }
//    if(EXTI_GetITStatus(EXTI_Line15) == SET) //如果是EXIT_11 出发的中断，则处理
//    {
//        for(i = 1000; i > 0; i--)
//            for(j = 720; j > 0; j--); //延迟一会，10ms
//        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == RESET) //消抖
//        {
//            key3.led_on_off = key3.led_on_off >= 3 ? 0 : key3.led_on_off + 1;
//            key3.key_change_bit = CHGE_IN;
//        }
//        EXTI_ClearITPendingBit(EXTI_Line15); //清除标志
//    }
}