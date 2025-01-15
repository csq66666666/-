#include "bsp_as5601.h"
#include <stdint.h>


#define abs(x) ((x)>0?(x):-(x))
#define _2PI 1
//#define BUFFER_SIZE 5
#define DIFFERENCE_THRESHOLD 5.0
static float angle_data_prev; //上次位置
static float full_rotation_offset; //转过的整圈数
//static uint16_t samplesBuffer[BUFFER_SIZE];
//static uint8_t bufferIndex = 0;
static float previousScaledAngle = 0.0;

#define Traverse_length   -360.0f //横移
#define Proportion   2.49f
static int t=1;


static int i2cWrite(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;
  
  status = HAL_I2C_Master_Transmit(&AS5601_I2C_HANDLE, dev_addr, pData, count, i2c_time_out);
  return status;
}

static int i2cRead(uint8_t dev_addr, uint8_t *pData, uint32_t count) {
  int status;
  int i2c_time_out = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;
  
  status = HAL_I2C_Master_Receive(&AS5601_I2C_HANDLE, (dev_addr | 1), pData, count, i2c_time_out);
  return status;
}

float bsp_as5601GetRawAngle(void) {
  uint16_t raw_angle;
  uint8_t buffer[2] = {0};
  uint8_t raw_angle_register = AS5601_RAW_ANGLE_REGISTER;
  
  i2cWrite(AS5601_ADDR, &raw_angle_register, 1);
  i2cRead(AS5601_ADDR, buffer, 2);
  raw_angle = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
	 float scaled_angle = ((float)raw_angle / (float)AS5601_RESOLUTION) * 360.0;
  return scaled_angle;
}



float distance2(void)
{
static float back_angle=0,back_angle_zone=0,d=0;

	if(t)
	{back_angle_zone=bsp_as5601GetAngle()/Proportion;
		t=0;
	}
	//装上后看是加还是减，现在按加写
	back_angle=(bsp_as5601GetAngle()/Proportion)-back_angle_zone;
	//添加滤波+
	d=back_angle*Traverse_length;

return d;



}
float bsp_as5601GetAngle(void) {
  float angle_data = bsp_as5601GetRawAngle();
  
  float d_angle = angle_data - angle_data_prev;
  if(abs(d_angle) > (0.6 *360.0)) {
    full_rotation_offset += (d_angle > 0 ? -_2PI : _2PI);
  }
  angle_data_prev = angle_data;
  
  return (full_rotation_offset + (angle_data / 360.0));
}


