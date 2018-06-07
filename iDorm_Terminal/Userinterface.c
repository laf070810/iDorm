
#include "userinterface.h"
#include "nokia5110.h"
#include <string.h>

/*        主界面函数                     */
void User_Home(void)
{
    char input[30];

	LCD_clear();
    LCD_write_english_string(0,0,"1.LIGHT");
    LCD_write_english_string(0,1,"2.STATE");
    LCD_write_english_string(0,2,"3.WATER");
    LCD_write_english_string(0,3,"4.INFO");
    LCD_write_english_string(0,4,"5.SETTING");

    WaitForInput(input);

    if (strcmp(input, "1") == 0)
		User_Light();
    else if (strcmp(input, "2") == 0)
        User_Status();
    else if (strcmp(input, "3") == 0)
        User_Water();
    else if (strcmp(input, "4") == 0)
        User_Info();
    else if (strcmp(input, "5") == 0)
        User_Config();
  }
/*       灯关控制                      */
void User_Light(void)
  {
	char input[30];

	LCD_clear();
    LCD_write_english_string(0,0,"1.CLOSE");
    LCD_write_english_string(0,1,"2.OPEN");
    LCD_write_english_string(0,2,"3.BACK");

	WaitForInput(input);

    if (strcmp(input, "3") == 0)
        return;
  }
/*       状态设置                    */
void User_Status(void)
{
	char input[30];

	LCD_clear();
    LCD_write_english_string(0,0,"1.NORMAL");
    LCD_write_english_string(0,1,"2.NOTDISTURB");
    LCD_write_english_string(0,2,"3.BACK");

	WaitForInput(input);

    if (strcmp(input, "3") == 0)
        return;
}
/*       送水需求                    */
void User_Water(void)
{
	char input[30], Reply[30];

	LCD_clear();
    LCD_write_english_string(0,0,"NUMBER: ");
	WaitForInput(input);
	Server_Water(input, Reply);
	LCD_write_english_string(0, 1, Reply);
	delay_ms(2000);
	return;
}

void User_Info(void)
{
	LCD_clear();
    LCD_write_english_string(0,0,"1.T:");
    //LCD_write_english_string(10,0,inputT);
    LCD_write_english_string(0,1,"2.Rh:");
    //LCD_write_english_string(10,1,inputRh);
    LCD_write_english_string(0,2,"3.Numberofbaths:");
    //LCD_write_english_string(0,3,inputbath);
    LCD_write_english_string(0,4,"4.BACK");
    //if (strcmp(input, "4") == 0)
   //     return;
}

void User_Config(void)
{
	char input[30], Reply[30];

	while (1)
	{
		LCD_clear();
		LCD_write_english_string(0, 0, "1.CONNECT");
		LCD_write_english_string(0, 1, "2.LOGIN");
		LCD_write_english_string(0, 2, "3.BACK");

		WaitForInput(input);

		if (strcmp(input, "1") == 0)
		{
			if (ESP8266_ConnectAP("Tsinghua", "") == 0)
			{
				ESP8266_TsinghuaLogin();
				delay_ms(1000);
				LCD_clear();
				LCD_write_english_string(0, 0, "Succeeded!");
				delay_ms(2000);
				continue;
			}
			else
			{
				LCD_clear();
				LCD_write_english_string(0, 0, "Failed!");
				delay_ms(2000);
				continue;
			}

		}
		else if (strcmp(input, "2") == 0)
		{
			Server_Login("601A", "1234", Reply);
			LCD_clear();
			LCD_write_english_string(0, 0, Reply);
			delay_ms(2000);
			continue;
		}	
		else if (strcmp(input, "3") == 0)
			return;
	}
}
/*
void SETTING_1(void)
  {
    LCD_write_english_string(0,0,"1.ACCOUNT");
    LCD_write_english_string(0,1,studentnumber);
    LCD_write_english_string(0,2,"2.PASSWORD");
    LCD_write_english_string(0,3,password1);
    LCD_write_english_string(0,4,"3.BACK");
  }

void SETTING_2(void)
 {
    LCD_write_english_string(0,0,"1.DORM");
    LCD_write_english_string(0,1,dromnumber);
    LCD_write_english_string(0,2,"2.PASSWORD");
    LCD_write_english_string(0,3,password2);
    LCD_write_english_string(0,4,"3.BACK");
    if (strcmp(input, "3") == 0)
        return;
 }
 */
void WaitForInput(char * input)
{
	UCA1_UART_RXBufLen = 0;
	UCA1_UART_RXBuf[UCA1_UART_RXBufLen] = 0;

	while (UCA1_UART_RXBufLen == 0);
	//while ((UCA1_UART_RXBuf[UCA1_UART_RXBufLen - 1]) != '\n');  //玄学：这样写无论如何都跳不出循环
	while (1)
	{
		if (UCA1_UART_RXBuf[UCA1_UART_RXBufLen - 1] != '\n')
			break;
	}
	input[0] = UCA1_UART_RXBuf[0];
	input[1] = 0;
}
