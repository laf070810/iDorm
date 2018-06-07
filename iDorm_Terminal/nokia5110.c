
#include "nokia5110.h"
#include "english_6x8_pixel.h" //дӢ��ͷ�ļ�

/*---------------------------------------------------------------------
LCD_init          : 5110LCD��ʼ��
---------------------------------------------------------------------*/
void LCD_init(void)
  {
  // ����һ����LCD��λ�ĵ͵�ƽ����
 
    LCD_5110_DIR |= (0x01 << LCD_RST) + (0x01 << LCD_SCE) + (0x01 << LCD_DC) 
      + (0x01 << LCD_DN) + (0x01<< LCD_SCLK);
 
    LCD_5110_OUT &= ~(0x01 << LCD_RST);
   
    // LCD_RST = 1;
    LCD_5110_OUT |= (0x01 << LCD_RST);
    
    // �ر�LCD
  
    LCD_5110_OUT &= ~(0x01 << LCD_SCE);
   
    // ʹ��LCD
    //LCD_SCE = 1;
    LCD_5110_OUT |= (0x01 << LCD_SCE);
 
    LCD_write_byte(0x21, 0); // ʹ����չ��������LCDģʽ
    LCD_write_byte(0xc8, 0); // ����ƫ�õ�ѹ
    LCD_write_byte(0x06, 0); // �¶�У��
    LCD_write_byte(0x13, 0); // 1:48
    LCD_write_byte(0x20, 0); // ʹ�û�������
    LCD_clear();         // ����
    LCD_write_byte(0x0c, 0); // �趨��ʾģʽ��������ʾ
        
    // �ر�LCD
    LCD_5110_OUT &= ~(0x01 << LCD_SCE);
  }

/*---------------------------------------------------------------------
LCD_clear         : LCD��������
---------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;
    LCD_write_byte(0x0c, 0);   
    LCD_write_byte(0x80, 0);   
    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);   
  }

/*---------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��
���������X       ��0��83
          Y       ��0��5
---------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);  // column
    LCD_write_byte(0x80 | X, 0);  // row
  }

/*---------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�
���������c       ����ʾ���ַ���
---------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;
    c -= 32;
    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }

/*---------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����
���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
---------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
    {
     LCD_write_char(*s);
     s++;
    }
  }

/*---------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD
���������data    ��д������ݣ�
          command ��д����/����ѡ��
---------------------------------------------------------------------*/
void LCD_write_byte(unsigned char data, unsigned char command)
  {
    unsigned char i;
    LCD_5110_OUT &= ~(0x01 << LCD_SCE); // msp430
    
    if (command == 0)                               // ��������
        LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else                                            // ��������
  LCD_5110_OUT |= (0x01 << LCD_DC);
  for(i=0;i<8;i++)
  {
   if(data&0x80)
    LCD_5110_OUT |= (0x01 << LCD_DN);
   else
      LCD_5110_OUT &= ~(0x01 << LCD_DN);
   //SCLK = 0;
   LCD_5110_OUT &= ~(0x01 << LCD_SCLK);
   data = data << 1;
   LCD_5110_OUT |= (0x01 << LCD_SCLK);
  }
    LCD_5110_OUT |= (0x01 << LCD_SCE);
  }

