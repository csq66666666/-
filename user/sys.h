#ifndef __SYS_H
#define __SYS_H	
#include "stm32f1xx_hal.h"


// 0: 不支持ucos, 1: 支持ucos
#define SYSTEM_SUPPORT_OS   0

// IO口操作宏定义
#define GPIO_PIN_ADDR(port, pin) ((GPIO_TypeDef*)(GPIOA_BASE + (port - GPIOA_BASE) * 32 + pin * 4))

#define PAout(n)   HAL_GPIO_WritePin(GPIOA, 1 << n, GPIO_PIN_SET)    // 输出
#define PAin(n)    HAL_GPIO_ReadPin(GPIOA, 1 << n)                   // 输入

#define PBout(n)   HAL_GPIO_WritePin(GPIOB, 1 << n, GPIO_PIN_SET)    // 输出
#define PBin(n)    HAL_GPIO_ReadPin(GPIOB, 1 << n)                   // 输入

#define PCout(n)   HAL_GPIO_WritePin(GPIOC, 1 << n, GPIO_PIN_SET)    // 输出
#define PCin(n)    HAL_GPIO_ReadPin(GPIOC, 1 << n)                   // 输入

#define PDout(n)   HAL_GPIO_WritePin(GPIOD, 1 << n, GPIO_PIN_SET)    // 输出
#define PDin(n)    HAL_GPIO_ReadPin(GPIOD, 1 << n)                   // 输入

#define PEout(n)   HAL_GPIO_WritePin(GPIOE, 1 << n, GPIO_PIN_SET)    // 输出
#define PEin(n)    HAL_GPIO_ReadPin(GPIOE, 1 << n)                   // 输入

#define PFout(n)   HAL_GPIO_WritePin(GPIOF, 1 << n, GPIO_PIN_SET)    // 输出
#define PFin(n)    HAL_GPIO_ReadPin(GPIOF, 1 << n)                   // 输入

#define PGout(n)   HAL_GPIO_WritePin(GPIOG, 1 << n, GPIO_PIN_SET)    // 输出
#define PGin(n)    HAL_GPIO_ReadPin(GPIOG, 1 << n)                   // 输入

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
//    // 新增条件
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
