/*
 * System.h
 *
 *  Created on: 2018年5月25日
 *      Author: LAF
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "msp430.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//USCI波特率
#define SMCLK_115200     0
#define SMCLK_9600      1
#define ACLK_9600       2

#define UCA0_UART_MODE       SMCLK_115200
#define UCA1_UART_MODE       SMCLK_9600

//延时函数
#define CPU_F ((double)16000000)   //MCLK 16MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

//系统初始化函数
void InitAll(void);
uint16_t setVCoreUp(uint8_t level);
bool increaseVCoreToLevel2();
void initClockTo16MHz(void);

//USCI函数
void UCA0_UART_Init(void);
void UCA1_UART_Init(void);
void UCA0_UART_SendString(char *);
void UCA1_UART_SendString(char *);

#endif /* SYSTEM_H_ */
