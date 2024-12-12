#include "bsp_as5600.h"
#include <stdint.h>


#define abs(x) ((x)>0?(x):-(x))
#define _2PI 1
//#define BUFFER_SIZE 5
#define DIFFERENCE_THRESHOLD 5.0

#define Proportion   1.587f
#define Front_rise_length  -250.1f //前伸

static int t=1;
static float angle_data_prev; //上次位置
static float full_rotation_offset; //转过的整圈数

static float previousScaledAngle = 6;




static int i2cWrite(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;
  
  status = HAL_I2C_Master_Transmit(&AS5600_I2C_HANDLE, dev_addr, pData, count, i2c_time_out);
  return status;
}

static int i2cRead(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;
  
  status = HAL_I2C_Master_Receive(&AS5600_I2C_HANDLE, (dev_addr | 1), pData, count, i2c_time_out);
  return status;
}

float bsp_as5600GetRawAngle(void) {
  uint16_t raw_angle;
  uint8_t buffer[2] = {0};
  uint8_t raw_angle_register = AS5600_RAW_ANGLE_REGISTER;
  
  i2cWrite(AS5600_ADDR, &raw_angle_register, 1);
  i2cRead(AS5600_ADDR, buffer, 2);
  raw_angle = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
	 float scaled_angle = ((float)raw_angle / (float)AS5600_RESOLUTION) * 360.0; 
  return scaled_angle;
}




float distance(void)
{
static float back_angle=0,back_angle_o=0,d=0;

	if(t)
	{back_angle_o=bsp_as5600GetAngle()/Proportion;
		t=0;
	}
	back_angle=(bsp_as5600GetAngle()/Proportion)-back_angle_o;

	d=back_angle*Front_rise_length;	
return d;
}

float bsp_as5600GetAngle(void) {
   float angle_data = bsp_as5600GetRawAngle();
  
  float d_angle = angle_data - angle_data_prev;
  if(abs(d_angle) > (0.6 * 360.0)) {
    full_rotation_offset += (d_angle > 0 ? -_2PI : _2PI);
  }
  angle_data_prev = angle_data;
  
  return (full_rotation_offset + (angle_data / 360.0));
}

