#include "down_1.h"
#include "Delay.h"
#include "gpio.h"

#include <stdint.h>
/**********************************************************
	STĞ¾Æ¬µ¥Æ¬»úHAL¿âÖ®IICÈí¼şÄ£Äâ 
	×÷Õß£ºÊ®Ò»
**********************************************************/
/**********************************************************
   1.IICÈí¼şÄ£Äâ   Ê¹ÓÃHAL¿âÊ±
   2.ĞèÒªSTM32CubeMXÅäÖÃ³õÊ¼»¯µÄÏà¹ØÒı½ÅÎªGPIOÄ£Ê½ SDA SCL³õÊ¼×´Ì¬ÏÂ¶¼ÊÇÊä³ö ÍÆÍì ÉÏÀ­Ä£Ê½ 
   4.³õÊ¼×´Ì¬ÏÂSDA Óë SCLÒª¸ø¸ßµçÆ½ Ê¹ÓÃ¸ßµÍµçÆ½×ª»»Ê±Ö®¼äÒªÓĞÃ÷ÏÔµÄus¼¶ÑÓÊ±
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.Òı½ÅÅäÖÃ ºê¶¨ÒåÓÃIFÓï¾ä 
2.¸øÒı½ÅµçÆ½±ØĞëÒª¸øÊä³öÄ£Ê½
3.SCLÒ»Ö±¶¼ÊÇÊä³öÄ£Ê½(Êä³öÊ±ÖÓ¿Ï¶¨ÊÇÊä³öÄ£Ê½)
4.ºê¶¨Òå°ó¶¨Òı½ÅSDAÓëSCL   SDA PB7    SCL PB6
**********************************************************/
#define SCL_Type     GPIOC
#define SDA_Type     GPIOC
#define abs(x) ((x)>0?(x):-(x))
#define SCL_GPIO    GPIO_PIN_14
#define SDA_GPIO    GPIO_PIN_15
//ÉèÖ3ÃÊä³ö¸ßµÍµçÆ½Ä£Ê½
#define SDA_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SDA_Type, SDA_GPIO, GPIO_PIN_RESET);
 
#define SCL_OUT(X)   if(X) \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(SCL_Type, SCL_GPIO, GPIO_PIN_RESET);    
                                     
#define SDA_IN		 HAL_GPIO_ReadPin(SDA_Type,SDA_GPIO)//Ö»ÓĞÊäÈëÄ£Ê½²ÅÄÜ¶ÁÈ¡µçÆ½×´Ì¬
 
/*****************************************
  SDAÒı½Å×ª±äÎª OUTÊä³öÄ£Ê½(Êä³öÄ£Ê½¸øÍ£Ö¹ ¿ªÊ¼ĞÅºÅ) 
******************************************/
static void IIC_SDA_Mode_OUT1(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  SDAÒı½Å×ª±äÎª ÊäÈëÄ£Ê½(ÊäÈëÄ£Ê½´«Êä¾ßÌåµÄÊı¾İ) 
******************************************/
static void IIC_SDA_Mode_IN1(void)
{
  GPIO_InitStruct.Pin = SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SDA_Type, &GPIO_InitStruct);
}
/*****************************************
  IIC¿ªÊ¼ĞÅºÅ
******************************************/
static void IIC_Start1(void)//IIC¿ªÊ¼ĞÅºÅ
{
	//ÉèÖÃÎªÊä³öÄ£Ê½
	IIC_SDA_Mode_OUT1();
	
	//¿ÕÏĞ×´Ì¬Á½¸öÒı½ÅÊÇ¸ßµçÆ½
	SDA_OUT(1);
	SCL_OUT(1) ;
	delay_us(5);
	
	//À­µÍÊı¾İÏß
	SDA_OUT(0);
	delay_us(5);
	
	//ÔÙÀ­µÍÊ±ÖÓÏß
	SCL_OUT(0) ;
	delay_us(5);
}
//IICÍ£Ö¹ĞÅºÅ
static void IIC_Stop1(void)
{
	//ÉèÖÃÎªÊä³öÄ£Ê½
	IIC_SDA_Mode_OUT1();
 
	//À­µÍ
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	//Ê±ÖÓÏßÏÈÀ­¸ß
	SCL_OUT(1);
	delay_us(5);
	
	//ÔÙ°ÑÊı¾İÏßÀ­¸ß
	SDA_OUT(1);
	delay_us(5);
}
static void IIC_Send_Byte1(uint8_t d)//Ö÷»ú·¢ËÍ8Î»Êı¾İ¸ø´Ó»úMSB ¸ßÎ»ÏÈ·¢
{
    uint8_t i = 0;
    	//ÉèÖÃÎªÊä³öÄ£Ê½
	IIC_SDA_Mode_OUT1();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//±íÊ¾Êı¾İÊÇ1
			SDA_OUT(1)
		else
			SDA_OUT(0);
		
		delay_us(5);
		SCL_OUT(1);//À­¸ßÊ±ÖÓÏß£¬¸æËß¶Ô·½Äã¿ÉÒÔ¶ÁÁË
		
		delay_us(5);
		SCL_OUT(0);//À­µÍÊ±ÖÓÏß£¬¸æËß¶Ô·½ÄãÔİÊ±±ğ¶Á£¬ÎÒÔÚ×¼±¸Êı¾İ
	}
 
}
static uint8_t IIC_Wait_Ack1(void)//µÈ´ı´Ó»ú¸øÖ÷»úÓ¦´ğ»òÕß²»Ó¦´ğ
{
	uint8_t ack = 0;
	//ÉèÖÃÎªÊäÈëÄ£Ê½
	IIC_SDA_Mode_IN1();
	
	//Ê±ÖÓÏßÀ­¸ß,Ê±ÖÓÏßÎª¸ßµçÆ½ÆÚ¼ä£¬²»¹ÜÊÇÊı¾İ»¹ÊÇack¶¼ÊÇÓĞĞ§µÄ
	SCL_OUT(1);
	delay_us(5);
	
	if( SDA_IN  == 1)
		ack = 1;//ÎŞĞ§ACK£¬¾ÍÊÇÎŞĞ§Ó¦´ğ
	else
		ack = 0;//ÓĞĞ§ACK£¬¾ÍÊÇÓĞĞ§Ó¦´ğ
	
	SCL_OUT(0);
    
	delay_us(5);
	return ack;
}
static uint8_t  IIC_Read_Byte1(void)//´Ó»ú·¢ËÍ8Î»Êı¾İ¸øÖ÷»ú
{
	uint8_t i =0;
	uint8_t data = 0;
	//ÉèÖÃÎªÊäÈëÄ£Ê½
	IIC_SDA_Mode_IN1();
	//ÏÈÀ­µÍÊ±ÖÓÏß£¬×¼±¸Êı¾İ
	SCL_OUT(0);
	delay_us(5);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(1);//Ê±ÖÓÏßÎª¸ßµçÆ½ÆÚ¼äÊı¾İ²ÅÊÇÓĞĞ§µÄ
		delay_us(5);
		if(SDA_IN == 1)
			data |= (0x1<<(7-i));//Êı¾İ¾ÍÊÇ1
		else
			data &= ~(0x1<<(7-i));//Êı¾İ¾ÍÊÇ0
		
		SCL_OUT (0);//¸æËß¶Ô·½´ËÊ±×¼±¸Êı¾İ£¬ÏÈ±ğ¶ÁĞ´
		delay_us(5);
	}
	return data;
}
static void IIC_Ack1(uint8_t ack)//Ö÷»ú·¢ËÍÓ¦´ğ»òÕß²»Ó¦´ğ¸ø´Ó»ú
{
	//ÉèÖÃÎªÊä³öÄ£Ê½
	IIC_SDA_Mode_OUT1();
	
	SDA_OUT(0);
	SCL_OUT(0);
	delay_us(5);
	
	SDA_OUT(ack) ;//·¢ËÍ¸ß/µÍµçÆ½--->·¢ËÍ²»Ó¦´ğ/Ó¦´ğ
	delay_us(5);
	
	SCL_OUT(1);//¸æËß´Ó»úÎÒÒÑ¾­×¼±¸ºÃÊı¾İ£¬Äã¿ÉÒÔ¶ÁÈ¡ÁË
	delay_us(5);
	
	SCL_OUT (0);//À­µÍÊ±ÖÓÏß£¬·¢ËÍack½áÊø
	delay_us(5);
}
static uint8_t AS5600_ReadOneByte1(uint8_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start1();  
	IIC_Send_Byte1((0X36<<1)|0x00);	  
	IIC_Wait_Ack1(); 
  IIC_Send_Byte1(ReadAddr);   
	IIC_Wait_Ack1();	    
	IIC_Start1();  	 	   
	IIC_Send_Byte1((0X36<<1)|0x01);           		   
	IIC_Wait_Ack1();	 
  temp=IIC_Read_Byte1();		   
  IIC_Stop1();	    
	return temp;
}
static uint16_t AS5600_ReadTwoByte1(void)
{
	uint16_t TwoByte_Data=0;
uint8_t hi_Data=0,lo_Data=0;
	hi_Data=AS5600_ReadOneByte1(0x0c);
	lo_Data=AS5600_ReadOneByte1(0x0d);
	TwoByte_Data = (hi_Data<<8)|lo_Data;
	return TwoByte_Data;
}

static float angle_data_prev_down;
static float full_rotation_offset_down;
 float bsp_as5666GetAngle_down(void) {

float angle_data1 =AS5600_ReadTwoByte1( );

float d_angle = angle_data1 - angle_data_prev_down;
  if(abs(d_angle) > 2048.0) {
    full_rotation_offset_down += (d_angle > 0 ? -1.0 : 1.0);
  }
  angle_data_prev_down = angle_data1;

 return (full_rotation_offset_down + (angle_data1 / 4096.0));



}
