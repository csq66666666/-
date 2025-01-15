#include "up_1.h"
#include "Delay.h"
#include "gpio.h"

#include <stdint.h>
/**********************************************************
	STоƬ��Ƭ��HAL��֮IIC���ģ�� 
	���ߣ�ʮһ
**********************************************************/
/**********************************************************
   1.IIC���ģ��   ʹ��HAL��ʱ
   2.��ҪSTM32CubeMX���ó�ʼ�����������ΪGPIOģʽ SDA SCL��ʼ״̬�¶������ ���� ����ģʽ 
   4.��ʼ״̬��SDA �� SCLҪ���ߵ�ƽ ʹ�øߵ͵�ƽת��ʱ֮��Ҫ�����Ե�us����ʱ
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.�������� �궨����IF��� 
2.�����ŵ�ƽ����Ҫ�����ģʽ
3.SCLһֱ�������ģʽ(���ʱ�ӿ϶������ģʽ)
4.�궨�������SDA��SCL   SDA PB7    SCL PB6
**********************************************************/
#define SCL_Type     GPIOA
#define SDA_Type     GPIOA
#define abs(x) ((x)>0?(x):-(x))
#define SCL_GPIO    GPIO_PIN_6
#define SDA_GPIO    GPIO_PIN_7
//��������ߵ͵�ƽģʽ
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//ֻ������ģʽ���ܶ�ȡ��ƽ״̬
 
/*****************************************
  SDA����ת��Ϊ OUT���ģʽ(���ģʽ��ֹͣ ��ʼ�ź�) 
******************************************/
static void IIC_SDA_Mode_OUT(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  SDA����ת��Ϊ ����ģʽ(����ģʽ������������) 
******************************************/
static void IIC_SDA_Mode_IN(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  IIC��ʼ�ź�
******************************************/
static void IIC_Start(void)//IIC��ʼ�ź�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	//����״̬���������Ǹߵ�ƽ
	SDA_OUT(1);
	SCL_OUT(1) ;
	delay_us(5);
	
	//����������
	SDA_OUT(0);
	delay_us(5);
	
	//������ʱ����
	SCL_OUT(0) ;
	delay_us(5);
}
//IICֹͣ�ź�
static void IIC_Stop(void)
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
 
	//����
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	//ʱ����������
	SCL_OUT(1);
	delay_us(5);
	
	//�ٰ�����������
	SDA_OUT(1);
	delay_us(5);
}
static void IIC_Send_Byte(uint8_t d)//��������8λ���ݸ��ӻ�MSB ��λ�ȷ�
{
    uint8_t i = 0;
    	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//��ʾ������1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		delay_us(5);
		SCL_OUT(1);//����ʱ���ߣ����߶Է�����Զ���
		
		delay_us(5);
		SCL_OUT(0);//����ʱ���ߣ����߶Է�����ʱ���������׼������
	}
 
}
static uint8_t IIC_Wait_Ack(void)//�ȴ��ӻ�������Ӧ����߲�Ӧ��
{
	uint8_t ack = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	
	//ʱ��������,ʱ����Ϊ�ߵ�ƽ�ڼ䣬���������ݻ���ack������Ч��
	SCL_OUT(1);
	delay_us(5);
	
	if( SDA_IN  == 1)
		ack = 1;//��ЧACK��������ЧӦ��
	else
		ack = 0;//��ЧACK��������ЧӦ��
	
	SCL_OUT(0);
    
	delay_us(5);
	return ack;
}
static uint8_t  IIC_Read_Byte(void)//�ӻ�����8λ���ݸ�����
{
	uint8_t i =0;
	uint8_t data = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN();
	//������ʱ���ߣ�׼������
	SCL_OUT(0);
	delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//ʱ����Ϊ�ߵ�ƽ�ڼ����ݲ�����Ч��
		delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//���ݾ���1
		else
			data &= ~(0x1<<(7-i));//���ݾ���0
		
		SCL_OUT (0);//���߶Է���ʱ׼�����ݣ��ȱ��д
		delay_us(5);
	}
	return data;
}
static void IIC_Ack(uint8_t ack)//��������Ӧ����߲�Ӧ����ӻ�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	SDA_OUT(ack) ;//���͸�/�͵�ƽ--->���Ͳ�Ӧ��/Ӧ��
	delay_us(5);
	
	SCL_OUT(1);//���ߴӻ����Ѿ�׼�������ݣ�����Զ�ȡ��
	delay_us(5);
	
	SCL_OUT (0);//����ʱ���ߣ�����ack����
	delay_us(5);
}
static uint8_t AS5600_ReadOneByte(uint8_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte((0X36<<1)|0x00);	  
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr);   
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte((0X36<<1)|0x01);           		   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte();		   
  IIC_Stop();	    
	return temp;
}
static uint16_t AS5600_ReadTwoByte(void)
{
	uint16_t TwoByte_Data=0;
uint8_t hi_Data=0,lo_Data=0;
	hi_Data=AS5600_ReadOneByte(0x0c);
	lo_Data=AS5600_ReadOneByte(0x0d);
	TwoByte_Data = (hi_Data<<8)|lo_Data;
	return TwoByte_Data;
}

static float angle_data_prev_up;
static float full_rotation_offset_up;
 float bsp_as5666GetAngle_up(void) {

float angle_data1 =AS5600_ReadTwoByte( );

float d_angle = angle_data1 - angle_data_prev_up;
  if(abs(d_angle) > 2048.0) {
    full_rotation_offset_up += (d_angle > 0 ? -1.0 : 1.0);
  }
  angle_data_prev_up = angle_data1;

 return (full_rotation_offset_up + (angle_data1 / 4096.0));



}
