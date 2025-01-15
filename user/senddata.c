// senddatatask.c

#include "senddata.h"
#include "CRC8_CRC16.h"
#include "string.h"
#include "usart.h"
#include "bsp_as5600.h"
#include "bsp_as5601.h"
#include "stdio.h"
#include "pitch_1.h"
#include "yaw_roll.h"
#include "tim.h"

void customDelay(uint32_t milliseconds)
{
    uint32_t start_time = HAL_GetTick();
    while ((HAL_GetTick() - start_time) < milliseconds)
    {
        // �ȴ�ָ��ʱ��
    }
}
void Float_HEX (float fdata, unsigned char *hdata)
{  if (hdata != NULL && fdata != NULL)
	memcpy(hdata,&fdata,sizeof(fdata));
}

//static void Change(unsigned char *result)
//{
//    float fdata = bsp_as5600GetRawAngle();
////	  float fdata1 = bsp_as5601GetRawAngle();
//    Float_HEX(fdata, result);
////	  Float_HEX(fdata1, result);
//}







//uint8_t data_buffer[1000];
//uint16_t buffer_index = 0;
//// ������������
//// ȫ�ֱ��������ڴ洢�����͵�����
//uint8_t data_from_buffer = 0;

// ������������
void SendDataTask(void)
{
   
 static uint8_t seq = 0;
    // ��������֡
    Controller_t tx_data;
    tx_data.frame_header.sof = 0xA5;                              // ����֡��ʼ�ֽڣ��̶�ֵΪ 0xA5
    tx_data.frame_header.data_length = DATA_LENGTH;               // ����֡�����ݶεĳ���
    tx_data.frame_header.seq = seq++;                                  // ����ţ�����ʵ��������д
    append_CRC8_check_sum((uint8_t *)(&tx_data.frame_header), 5); // ���֡ͷ CRC8 У��λ

    tx_data.cmd_id = CONTROLLER_CMD_ID; // ������ID������ʵ��������д

    // ��������䵽���ݶ���
    Float_HEX(roll_angle(), tx_data.data);
    Float_HEX(yaw_angle(), tx_data.data + 4);
    Float_HEX(pitch_angle(), tx_data.data + 8);
    Float_HEX(distance(), tx_data.data + 12);
    Float_HEX(distance2(), tx_data.data + 16);

    append_CRC16_check_sum((uint8_t *)(&tx_data), DATA_FRAME_LENGTH);

 

     HAL_UART_Transmit(&huart1, (uint8_t *)(&tx_data), sizeof(tx_data), 50);

 customDelay(0);

  
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // �ú����� stm32f1xx_hal_tim.c �ж���Ϊ������(__weak)�����û��ٶ���
{
	  if(htim == &htim2)
	 {
  
	SendDataTask();
	 }
}

