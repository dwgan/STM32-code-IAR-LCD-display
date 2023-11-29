#ifndef __GPIO_CONFIG_H_
#define __GPIO_CONFIG_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_13) //��LED0
#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_13)  //�ر�LED0

#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_14) //��LED1
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_14)  //�ر�LED1

#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_15) //��LED2
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_15)  //�ر�LED2

#define LED3_ON GPIO_ResetBits(GPIOA, GPIO_Pin_0)  //��LED3
#define LED3_OFF GPIO_SetBits(GPIOA, GPIO_Pin_0)   //�ر�LED3



extern void led_gpio_init(void);



#endif