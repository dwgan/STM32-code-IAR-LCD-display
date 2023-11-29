#ifndef __DHT11_H_
#define __DHT11_H_

#include "stm32f10x_gpio.h"
#include "delay.h"

#define DHT11_OUT_H GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define DHT11_OUT_L GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define DHT11_IN GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
extern u16 Data_Buff[5];
void dht11_portin(void);
void dht11_portout(void);
void dht11_reset(void);
u16 dht11_scan(void);
u16 dht11_read_bit(void);
u16 dht11_read_byte(void);
u16 dht11_read_data(void);




#endif
