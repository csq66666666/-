#ifndef __DELAY_H
#define __DELAY_H
 
#include "main.h"
 
void My_Delay_us_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void My_Delay_ms(uint32_t nms);
 
#endif

