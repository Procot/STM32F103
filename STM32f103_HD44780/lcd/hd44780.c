#include "hd44780.h"

void lcd_init(void)
{
	TIM2_init();
	
	lcd_init_pins();

	_delay_ms(40);
	
	lcd_send_pol_byte(0x3, COMMAND);
	
	_delay_ms(5);
	
	lcd_send_pol_byte(0x3, COMMAND);
	
	_delay_us(200); //more 100us
	
	lcd_send_pol_byte(0x3, COMMAND);
	
	_delay_us(200);

	lcd_send_pol_byte(0x2, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0x2, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0x8, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0x8, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0x1, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0x6, COMMAND);
	
	_delay_us(200);
	lcd_send_pol_byte(0, COMMAND);
	
	_delay_us(200);
	
	lcd_send_pol_byte(0xD, COMMAND);
	
	_delay_us(200);
	
	lcd_send_byte(0x80, COMMAND);
}

void lcd_init_pins(void)
{
	GPIO_InitTypeDef sign, data;
	
	RCC_APB2PeriphClockCmd(RCC_PORT_SIG, ENABLE);
  sign.GPIO_Mode = GPIO_Mode_Out_PP;
  sign.GPIO_Pin = RS | EN;
  sign.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_SIG, &sign);

	RCC_APB2PeriphClockCmd(RCC_PORT_DATA, ENABLE);
  data.GPIO_Mode = GPIO_Mode_Out_PP;
  data.GPIO_Pin = D7 | D6 | D5 | D4;
  data.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_DATA, &data);
}

void lcd_send_byte(unsigned char p, BitAction bitVal)
{
	lcd_send_pol_byte(p >> 4, bitVal);
	lcd_send_pol_byte(p & 0x0F, bitVal);
}

void lcd_send_pol_byte(unsigned char c, BitAction bitVal)
{
	BitAction dataBits[4] = { Bit_RESET, Bit_RESET, Bit_RESET, Bit_RESET };
	for (int i = 0; i < 4; i++)
	{
		if (c & (1 << i))
		{
			dataBits[i] = Bit_SET;
		}
	}
	GPIO_WriteBit(PORT_SIG, RS, bitVal);
	
	GPIO_WriteBit(PORT_SIG, EN, Bit_SET);
	GPIO_WriteBit(PORT_DATA, D7, dataBits[3]);
	GPIO_WriteBit(PORT_DATA, D6, dataBits[2]);
	GPIO_WriteBit(PORT_DATA, D5, dataBits[1]);
	GPIO_WriteBit(PORT_DATA, D4, dataBits[0]);
	
	_delay_us(200);
	GPIO_WriteBit(PORT_SIG, EN, Bit_RESET);
	_delay_us(200);
}

void lcd_send_char(unsigned char c)
{
	lcd_send_byte(c, DATA);
	_delay_us(200);
}

void lcd_send_str(char* str)
{
	while (*str)
	{
		lcd_send_char(*str++);
	}
}

void lcd_send_enter(void)
{
	lcd_send_byte(0xC0, COMMAND);
	_delay_us(200);
}

void lcd_clear(void)
{
	lcd_send_byte(0x01, COMMAND);
	_delay_us(200);
}

void lcd_cursor_on_begin(void)
{
	lcd_send_byte(0x80, COMMAND);
	_delay_us(200);
}
