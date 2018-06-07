#include <msp430.h> 
#include "System.h"
#include "ESP8266.h"
#include "Userinterface.h"

/**
 * main.c
 */

char UCA0_UART_RXBuf[1000], UCA1_UART_RXBuf[1000];
int UCA0_UART_RXBufLen = 0, UCA1_UART_RXBufLen = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    InitAll();
	_bis_SR_register(GIE);

	P1DIR |= BIT2;
	P1OUT |= BIT2;

	while (1)
	{
		User_Home();
	}

/*
    int flag, i;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

	_bis_SR_register(GIE);
	if (ESP8266_ConnectAP("Tsinghua", "") != 0)
	{
	    P4OUT |= BIT7;
	    while (1);
	}
	ESP8266_TsinghuaLogin();
	Delay(1);
	ESP8266_ConnectServer("TCP", "183.172.146.224", "1234");
	Delay(1);
	while (1)
	{
	    UCA1TXBUF = 'a';
	    while (!(UCA1IFG & UCTXIFG));
	}

	while (1)
	{
	    UCA0_UART_RXBufLen = 0;
	    ESP8266_CIPSendString("BEGIN\nREGISTER\nUsername:601A\nPassword:123456789\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nREGISTER\nUsername:602A\nPassword:123456789\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nREGISTER\nUsername:603A\nPassword:123456789\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nLOGIN\nUsername:601A\nPassword:123456789\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nWATER\nUsername:601A\nNumber:1\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nLOGOUT\nUsername:601A\nPassword:123456789\nEND\n");
	    Delay(1);
	    ESP8266_CIPSendString("BEGIN\nLOGIN\nUsername:602A\nPassword:123456789\nEND\n");
	            Delay(1);
	            ESP8266_CIPSendString("BEGIN\nWATER\nUsername:602A\nNumber:1\nEND\n");
	            Delay(1);
	            ESP8266_CIPSendString("BEGIN\nLOGOUT\nUsername:602A\nPassword:123456789\nEND\n");
	            Delay(1);
	}

	if (flag == 0)
	{
	    P4OUT |= BIT7;
	    while (1);
	}
	else
	{
	    while (1)
	    {
	        P4OUT |= BIT7;
	        for (i = 0; i < 30000; i++);
	        P4OUT &= ~BIT7;
	        for (i = 0; i < 30000; i++);
	    }
	}
*/
	return 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    if (UCA0_UART_RXBufLen < sizeof(UCA0_UART_RXBuf) - 1)
        UCA0_UART_RXBuf[UCA0_UART_RXBufLen++] = UCA0RXBUF;
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCA1IV, 4))
	{
	case 0:break;                             // Vector 0 - no interrupt
	case 2:                                   // Vector 2 - RXIFG
		if (UCA1_UART_RXBufLen < sizeof(UCA1_UART_RXBuf) - 1)
			UCA1_UART_RXBuf[UCA1_UART_RXBufLen++] = UCA1RXBUF;
		break;
	case 4:break;                             // Vector 4 - TXIFG
	default: break;
	}
}
