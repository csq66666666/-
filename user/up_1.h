/*
 * as5600.h
 *
 *  Created on: Jan 19, 2023
 *      Author: Administrator
 */
 
#ifndef _UP_1_H
#define _UP_1_H
#include <stdint.h>
void IIC_SDA_Mode_OUT(void);
void IIC_SDA_Mode_IN(void);
 void IIC_Start(void);
 void IIC_Stop(void);
 void IIC_Send_Byte(uint8_t d);
 uint8_t IIC_Wait_Ack(void);
 uint8_t  IIC_Read_Byte(void);
 void IIC_Ack(uint8_t ack);
uint8_t AS5600_ReadOneByte(uint8_t ReadAddr);
uint16_t AS5600_ReadTwoByte(void);
 float bsp_as5666GetAngle_up(void);
#endif
