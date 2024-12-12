#ifndef __BSP_AS5601_H
#define __BSP_AS5601_H

#include "i2c.h"

#define AS5601_I2C_HANDLE hi2c1

#define I2C_TIME_OUT_BASE   10
#define I2C_TIME_OUT_BYTE   1

/*
ע��:AS5600�ĵ�ַ0x36��ָ����ԭʼ7λ�豸��ַ,��ST I2C���е��豸��ַ��ָԭʼ�豸��ַ����һλ�õ����豸��ַ
*/

#define AS5601_RAW_ADDR    0x36
#define AS5601_ADDR        (AS5601_RAW_ADDR << 1)
#define AS5601_WRITE_ADDR  (AS5601_RAW_ADDR << 1)
#define AS5601_READ_ADDR   ((AS5601_RAW_ADDR << 1) | 1)


#define AS5601_RESOLUTION 4096 //12bit Resolution 

#define AS5601_RAW_ANGLE_REGISTER  0x0C


void bsp_as5601Init(void);
float bsp_as5601GetRawAngle(void);
float bsp_as5601GetAngle(void);
float distance2(void);
#endif /* __BSP_AS5600_H */


