/*
 * Server.c
 *
 *  Created on: 2018Äê6ÔÂ3ÈÕ
 *      Author: LAF
 */

#include "Server.h"

void Server_Login(char * Username, char * Password, char * Reply)
{
	char tmp[100] = "BEGIN\nLOGIN\nUsername:";
	strcat(tmp, Username);
	strcat(tmp, "\nPassword:");
	strcat(tmp, Password);
	strcat(tmp, "\nEND\n");
	ESP8266_ConnectServer("TCP", SERVERADDR, SERVERPORT);
	delay_ms(1000);
	UCA0_UART_RXBufLen = 0;
	ESP8266_CIPSendString(tmp);
	delay_ms(3000);
	UCA0_UART_RXBuf[UCA0_UART_RXBufLen] = 0;
	strcpy(Reply, strstr(UCA0_UART_RXBuf, "login"));
}

void Server_Water(char * input, char * Reply)
{
	char tmp[100] = "BEGIN\nWATER\nUsername:601A\nNumber:";
	strcat(tmp, input);
	strcat(tmp, "\nEND\n");
	ESP8266_ConnectServer("TCP", SERVERADDR, SERVERPORT);
	delay_ms(1000);
	UCA0_UART_RXBufLen = 0;
	ESP8266_CIPSendString(tmp);
	delay_ms(3000);
	UCA0_UART_RXBuf[UCA0_UART_RXBufLen] = 0;
	strcpy(Reply, strstr(UCA0_UART_RXBuf, "call"));
}


