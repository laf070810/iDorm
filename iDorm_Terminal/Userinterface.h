
#ifndef __Userinterface_h_
#define __Userinterface_h_

#include "msp430.h"
#include "System.h"

extern char UCA0_UART_RXBuf[];
extern int UCA0_UART_RXBufLen;
extern char UCA1_UART_RXBuf[];
extern int UCA1_UART_RXBufLen;

void  User_Home(void);
void  User_Light(void);
void  User_Status(void);
void  User_Water(void);
void  User_Info(void);
void  User_Config(void);
//void  SETTING_1(void);
//void  SETTING_2(void);

void WaitForInput(char * input);
#endif
