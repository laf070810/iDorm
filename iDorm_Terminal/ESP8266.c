/*
 * ESP8266.c
 *
 *  Created on: 2018年5月25日
 *      Author: LAF
 */

#include "ESP8266.h"

void Num_To_Str(int Num, char * Str)
{
    int i = 0, Len;
    char Tmp;

    while (Num > 0)
    {
        Str[i++] = Num % 10 + '0';
        Num /= 10;
    }
    Str[i] = 0;
    Len = strlen(Str);
    for (i = 0; i < Len / 2; i++)
    {
        Tmp = Str[Len - i - 1];
        Str[Len - i - 1] = Str[i];
        Str[i] = Tmp;
    }
}

int ESP8266_ConnectAP(char * SSID, char * Password)
{
    while (1)
    {
        UCA0_UART_RXBufLen = 0;

        UCA0_UART_SendString("AT+CWJAP_DEF=\"");
        UCA0_UART_SendString(SSID);
        UCA0_UART_SendString("\",\"");
        UCA0_UART_SendString(Password);
        UCA0_UART_SendString("\"\r\n");

		delay_ms(10000);
        UCA0_UART_RXBuf[UCA0_UART_RXBufLen] = 0;
        if (strstr(UCA0_UART_RXBuf, "OK") != NULL)
            return 0;
        else if (strstr(UCA0_UART_RXBuf, "FAIL") != NULL)
            return 1;
    }
}

int ESP8266_TsinghuaLogin(void)
{
    char LoginPacket[300] = {"POST /do_login.php HTTP/1.1\r\n\
Host: net.tsinghua.edu.cn\r\n\
Connection: keep-alive\r\n\
Content-Length: 86\r\n\
Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n\r\n"};

    int Retry = 3;

    //建立TCP连接,失败则返回1
    if (ESP8266_ConnectServer("TCP", "net.tsinghua.edu.cn", "80") != 0)
        return 1;
    //发送登录数据包,重试最多3次，都失败则返回2
    strcat(LoginPacket, "action=login&username=laf17&password={MD5_HEX}9aa4c7ebeba583e7b36d30a172dfc0bb&ac_id=1\r\n");
    Retry = 3;
    while (1)
    {
        UCA0_UART_RXBufLen = 0;
        ESP8266_CIPSendString(LoginPacket);
		delay_ms(3000);
        UCA0_UART_RXBuf[UCA0_UART_RXBufLen] = 0;
        if ((strstr(UCA0_UART_RXBuf, "Login is successful") != NULL) || (strstr(UCA0_UART_RXBuf, "IP has been online") != NULL))
            break;
        else if (Retry > 0)
        {
            Retry--;
            continue;
        }
        else
            return 2;
    }
    UCA0_UART_SendString("AT+CIPCLOSE\r\n");
    return 0;
}

int ESP8266_ConnectServer(char * Protocol, char * IPAddr, char * Port)
{
    int Retry = 3;

    while (1)
    {
        UCA0_UART_RXBufLen = 0;

        UCA0_UART_SendString("AT+CIPSTART=\"");
        UCA0_UART_SendString(Protocol);
        UCA0_UART_SendString("\",\"");
        UCA0_UART_SendString(IPAddr);
        UCA0_UART_SendString("\",");
        UCA0_UART_SendString(Port);
        UCA0_UART_SendString("\r\n");
		delay_ms(1000);

        UCA0_UART_RXBuf[UCA0_UART_RXBufLen] = 0;
        if (strstr(UCA0_UART_RXBuf, "OK") != NULL)
            break;
        else if (Retry > 0)
        {
            Retry--;
            continue;
        }
        else
            return 1;
    }

    return 0;
}

void ESP8266_CIPSendString(char * Str)
{
    char Len[5];

    Num_To_Str(strlen(Str), Len);
    UCA0_UART_SendString("AT+CIPSEND=");
    UCA0_UART_SendString(Len);
    UCA0_UART_SendString("\r\n");
	delay_ms(1000);
    UCA0_UART_SendString(Str);
}
