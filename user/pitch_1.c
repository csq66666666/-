#include "pitch_1.h"
#include "Delay.h"
#include "gpio.h"

#include <stdint.h>
/**********************************************************
	ST芯片单片机HAL库之IIC软件模拟 
	作者：十一
**********************************************************/
/**********************************************************
   1.IIC软件模拟   使用HAL库时
   2.需要STM32CubeMX配置初始化的相关引脚为GPIO模式 SDA SCL初始状态下都是输出 推挽 上拉模式 
   4.初始状态下SDA 与 SCL要给高电平 使用高低电平转换时之间要有明显的us级延时
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.引脚配置 宏定义用IF语句 
2.给引脚电平必须要给输出模式
3.SCL一直都是输出模式(输出时钟肯定是输出模式)
4.宏定义绑定引脚SDA与SCL   SDA PB7    SCL PB6
**********************************************************/
#define SCL_Type     GPIOB
#define SDA_Type     GPIOB
#define abs(x) ((x)>0?(x):-(x))
#define SCL_GPIO    GPIO_PIN_14
#define SDA_GPIO    GPIO_PIN_15
//设置输出高低电平模式
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//只有输入模式才能读取电平状态
 static int t=1;
/*****************************************
  SDA引脚转变为 OUT输出模式(输出模式给停止 开始信号) 
******************************************/
void IIC_SDA_Mode_OUT2(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  SDA引脚转变为 输入模式(输入模式传输具体的数据) 
******************************************/
void IIC_SDA_Mode_IN2(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  IIC开始信号
******************************************/
void IIC_Start2(void)//IIC开始信号
{
	//设置为输出模式
	IIC_SDA_Mode_OUT2();
	
	//空闲状态两个引脚是高电平
	SDA_OUT(1);
	SCL_OUT(1) ;
	delay_us(5);
	
	//拉低数据线
	SDA_OUT(0);
	delay_us(5);
	
	//再拉低时钟线
	SCL_OUT(0) ;
	delay_us(5);
}
//IIC停止信号
void IIC_Stop2(void)
{
	//设置为输出模式
	IIC_SDA_Mode_OUT2();
 
	//拉低
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	//时钟线先拉高
	SCL_OUT(1);
	delay_us(5);
	
	//再把数据线拉高
	SDA_OUT(1);
	delay_us(5);
}
void IIC_Send_Byte2(uint8_t d)//主机发送8位数据给从机MSB 高位先发
{
    uint8_t i = 0;
    	//设置为输出模式
	IIC_SDA_Mode_OUT2();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//表示数据是1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		delay_us(5);
		SCL_OUT(1);//拉高时钟线，告诉对方你可以读了
		
		delay_us(5);
		SCL_OUT(0);//拉低时钟线，告诉对方你暂时别读，我在准备数据
	}
 
}
uint8_t IIC_Wait_Ack2(void)//等待从机给主机应答或者不应答
{
	uint8_t ack = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN2();
	
	//时钟线拉高,时钟线为高电平期间，不管是数据还是ack都是有效的
	SCL_OUT(1);
	delay_us(5);
	
	if( SDA_IN  == 1)
		ack = 1;//无效ACK，就是无效应答
	else
		ack = 0;//有效ACK，就是有效应答
	
	SCL_OUT(0);
    
	delay_us(5);
	return ack;
}
uint8_t  IIC_Read_Byte2(void)//从机发送8位数据给主机
{
	uint8_t i =0;
	uint8_t data = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN2();
	//先拉低时钟线，准备数据
	SCL_OUT(0);
	delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//时钟线为高电平期间数据才是有效的
		delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//数据就是1
		else
			data &= ~(0x1<<(7-i));//数据就是0
		
		SCL_OUT (0);//告诉对方此时准备数据，先别读写
		delay_us(5);
	}
	return data;
}
void IIC_Ack2(uint8_t ack)//主机发送应答或者不应答给从机
{
	//设置为输出模式
	IIC_SDA_Mode_OUT2();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	SDA_OUT(ack) ;//发送高/低电平--->发送不应答/应答
	delay_us(5);
	
	SCL_OUT(1);//告诉从机我已经准备好数据，你可以读取了
	delay_us(5);
	
	SCL_OUT (0);//拉低时钟线，发送ack结束
	delay_us(5);
}
uint8_t AS5600_ReadOneByte2(uint8_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start2();  
	IIC_Send_Byte2((0X36<<1)|0x00);	  
	IIC_Wait_Ack2(); 
  IIC_Send_Byte2(ReadAddr);   
	IIC_Wait_Ack2();	    
	IIC_Start2();  	 	   
	IIC_Send_Byte2((0X36<<1)|0x01);           		   
	IIC_Wait_Ack2();	 
  temp=IIC_Read_Byte2();		   
  IIC_Stop2();	    
	return temp;
}
uint16_t AS5600_ReadTwoByte2(void)
{
	uint16_t TwoByte_Data=0;
uint8_t hi_Data=0,lo_Data=0;
	hi_Data=AS5600_ReadOneByte2(0x0c);
	lo_Data=AS5600_ReadOneByte2(0x0d);
	TwoByte_Data = (hi_Data<<8)|lo_Data;
	return TwoByte_Data;
}

float angle_data_prev_pitch;
float full_rotation_offset_pitch;
float bsp_as5666GetAngle_pitch(void) {

float angle_data1 =AS5600_ReadTwoByte2( );

float d_angle = angle_data1 - angle_data_prev_pitch;
  if(abs(d_angle) > 2048.0) {
    full_rotation_offset_pitch += (d_angle > 0 ? -1.0 : 1.0);
  }
  angle_data_prev_pitch = angle_data1;
  	

 return (full_rotation_offset_pitch + (angle_data1 / 4096.0));
}

static float  pitch_angle_init;
static float  pitch_angle_1;
static float  pitch_angle_0;
float pitch_angle(void){
if (t){
pitch_angle_init=bsp_as5666GetAngle_pitch();
	t=0;
}

 pitch_angle_0=bsp_as5666GetAngle_pitch();

pitch_angle_1=pitch_angle_0-pitch_angle_init;

return pitch_angle_1/3.94*360.0f
	;
}

