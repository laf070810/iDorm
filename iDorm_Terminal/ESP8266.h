/*
 * ESP8266.h
 *
 *  Created on: 2018Äê5ÔÂ26ÈÕ
 *      Author: LAF
 */

#include <string.h>
#include <System.h>

#ifndef ESP8266_H_
#define ESP8266_H_

extern char UCA0_UART_RXBuf[];
extern int UCA0_UART_RXBufLen;

void Num_To_Str(int, char *);
int ESP8266_ConnectAP(char * SSID, char * Password);
int ESP8266_TsinghuaLogin(void);
int ESP8266_ConnectServer(char * Protocol, char * IPAddr, char * Port);
void ESP8266_CIPSendString(char * Str);


#endif /* ESP8266_H_ */
