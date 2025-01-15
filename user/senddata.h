// senddatatask.h

#ifndef SENDDATA_H
#define SENDDATA_H

#include <stdint.h>
#define FRAME_HEADER_LENGTH 5 // 帧头数据长度
#define CMD_ID_LENGTH 2       // 命令码ID数据长度
#define DATA_LENGTH 30        // 数据段长度
#define FRAME_TAIL_LENGTH 2   // 帧尾数据长度

#define DATA_FRAME_LENGTH (FRAME_HEADER_LENGTH + CMD_ID_LENGTH + DATA_LENGTH + FRAME_TAIL_LENGTH) // 整个数据帧的长度

#define CONTROLLER_CMD_ID 0x0302 // 自定义控制器命令码

typedef __packed struct
{
    __packed struct
    {
        uint8_t sof;              // 起始字节，固定值为0xA5
        uint16_t data_length;     // 数据帧中data的长度
        uint8_t seq;              // 包序号
        uint8_t crc8;             // 帧头CRC8校验
    } frame_header;               // 帧头
    __packed uint16_t cmd_id;     // 命令码
    __packed uint8_t data[30];    // 自定义控制器的数据帧
    __packed uint16_t frame_tail; // 帧尾CRC16校验
} Controller_t;                   // 自定义控制器数据包

void SendDataTask_Init(void);
void SendDataTask(void);
void customDelay(uint32_t milliseconds);
void Float_HEX (float fdata, unsigned char *hdata);	
void Change(unsigned char *result1, unsigned char *result2,unsigned char *result3,unsigned char *result4,unsigned char *result5);
#endif // SENDDATATASK_H
