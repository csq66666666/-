#ifndef __BSP_AS5601_H
#define __BSP_AS5601_H

#include "i2c.h"

#define AS5601_I2C_HANDLE hi2c1

#define I2C_TIME_OUT_BASE   10
#define I2C_TIME_OUT_BYTE   1

/*
注意:AS5600的地址0x36是指的是原始7位设备地址,而ST I2C库中的设备地址是指原始设备地址左移一位得到的设备地址
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


