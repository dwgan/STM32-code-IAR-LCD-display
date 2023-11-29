#ifndef __GPIO_CONFIG_H_
#define __GPIO_CONFIG_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_13) //打开LED0
#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_13)  //关闭LED0

#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_14) //打开LED1
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_14)  //关闭LED1

#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_15) //打开LED2
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_15)  //关闭LED2

#define LED3_ON GPIO_ResetBits(GPIOA, GPIO_Pin_0)  //打开LED3
#define LED3_OFF GPIO_SetBits(GPIOA, GPIO_Pin_0)   //关闭LED3



extern void led_gpio_init(void);



#endif