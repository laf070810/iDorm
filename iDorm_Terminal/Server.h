/*
 * Server.h
 *
 *  Created on: 2018Äê6ÔÂ3ÈÕ
 *      Author: LAF
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "System.h"

extern char UCA0_UART_RXBuf[];
extern int UCA0_UART_RXBufLen;
extern char UCA1_UART_RXBuf[];
extern int UCA1_UART_RXBufLen;

#define SERVERADDR "101.5.144.34"
#define SERVERPORT "1234"

void Server_Water(char *, char *);

#endif /* SERVER_H_ */
