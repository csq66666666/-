#ifndef __SYS_H
#define __SYS_H	
#include "stm32f1xx_hal.h"


// 0: ��֧��ucos, 1: ֧��ucos
#define SYSTEM_SUPPORT_OS   0

// IO�ڲ����궨��
#define GPIO_PIN_ADDR(port, pin) ((GPIO_TypeDef*)(GPIOA_BASE + (port - GPIOA_BASE) * 32 + pin * 4))

#define PAout(n)   HAL_GPIO_WritePin(GPIOA, 1 << n, GPIO_PIN_SET)    // ���
#define PAin(n)    HAL_GPIO_ReadPin(GPIOA, 1 << n)                   // ����

#define PBout(n)   HAL_GPIO_WritePin(GPIOB, 1 << n, GPIO_PIN_SET)    // ���
#define PBin(n)    HAL_GPIO_ReadPin(GPIOB, 1 << n)                   // ����

#define PCout(n)   HAL_GPIO_WritePin(GPIOC, 1 << n, GPIO_PIN_SET)    // ���
#define PCin(n)    HAL_GPIO_ReadPin(GPIOC, 1 << n)                   // ����

#define PDout(n)   HAL_GPIO_WritePin(GPIOD, 1 << n, GPIO_PIN_SET)    // ���
#define PDin(n)    HAL_GPIO_ReadPin(GPIOD, 1 << n)                   // ����

#define PEout(n)   HAL_GPIO_WritePin(GPIOE, 1 << n, GPIO_PIN_SET)    // ���
#define PEin(n)    HAL_GPIO_ReadPin(GPIOE, 1 << n)                   // ����

#define PFout(n)   HAL_GPIO_WritePin(GPIOF, 1 << n, GPIO_PIN_SET)    // ���
#define PFin(n)    HAL_GPIO_ReadPin(GPIOF, 1 << n)                   // ����

#define PGout(n)   HAL_GPIO_WritePin(GPIOG, 1 << n, GPIO_PIN_SET)    // ���
#define PGin(n)    HAL_GPIO_ReadPin(GPIOG, 1 << n)                   // ����

#endif
//  if (a > 1 && a < 2) {
//        yaw_Angle -= angle_increment;
//    }
//    else if (a > 2 && a < 3) {
//        yaw_Angle -= 2 * angle_increment;
//    }
//    else if (a < -1 && a > -2) {
//        yaw_Angle += angle_increment;
//    }
//    else if (a < -2 && a > -3) {
//        yaw_Angle += 2 * angle_increment;
//    }
//    else if (a > 3 && a < 4) {
//        yaw_Angle -= 3 * angle_increment;
//    }
//    else if (a < -3 && a > -4) {
//        yaw_Angle += 3 * angle_increment;
//    }
//    // ��������
//    else if (a > 4 && a < 5) {
//        yaw_Angle -= 4 * angle_increment;
//    }
//    else if (a < -4 && a > -5) {
//        yaw_Angle += 4 * angle_increment;
//    }
//    else if (a > 5 && a < 6) {
//        yaw_Angle -= 5 * angle_increment;
//    }
//    else if (a < -4 && a > -5) {
//        yaw_Angle += 5 * angle_increment;
//    }
