/*
 * as5600.h
 *
 *  Created on: Jan 19, 2023
 *      Author: Administrator
 */
 
#ifndef _PITCH_1_H
#define _PITCH_1_H
#include <stdint.h>
void IIC_SDA_Mode_OUT2(void);
void IIC_SDA_Mode_IN2(void);
 void IIC_Start2(void);
 void IIC_Stop2(void);
 void IIC_Send_Byte2(uint8_t d);
 uint8_t IIC_Wait_Ack2(void);
 uint8_t  IIC_Read_Byte2(void);
 void IIC_Ack2(uint8_t ack);
uint8_t AS5600_ReadOneByte2(uint8_t ReadAddr);
uint16_t AS5600_ReadTwoByte2(void);
float bsp_as5666GetAngle_pitch(void);
float pitch_angle(void);
#endif
