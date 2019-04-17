#include "keyboard.h"

const unsigned char MATRIX[4][4] = {
	{ '1', '2', '3', 'A' },
	{ '4', '5', '6', 'B' },
	{ '7', '8', '9', 'C' },
	{ '*', '0', '#', 'D' }
};

const uint16_t ROWS[] = { ROW1, ROW2, ROW3, ROW4 };
const uint16_t COLS[] = { COL1, COL2, COL3, COL4 };

void keyboard_init(void)
{
	keyboard_pins_init();
}

void keyboard_pins_init(void)
{
	GPIO_InitTypeDef portRows, portCols;
	
	RCC_APB2PeriphClockCmd(RCC_PORT_ROWS, ENABLE);
  portRows.GPIO_Mode = GPIO_Mode_Out_PP;
  portRows.GPIO_Pin = ROW1 | ROW2 | ROW3 | ROW4;
  portRows.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_ROWS, &portRows);
	
	RCC_APB2PeriphClockCmd(RCC_PORT_COLS, ENABLE);
  portCols.GPIO_Mode = GPIO_Mode_IPD;
  portCols.GPIO_Pin = COL1 | COL2 | COL3 | COL4;
  portCols.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_COLS, &portCols);
}

unsigned char keyboard_key_press(void)
{
	while(1)
	{
		for(uint8_t i = 0; i < 4; ++i)
		{
			GPIO_SetBits(PORT_ROWS, ROWS[i]);
			for (uint8_t j = 0; j < 4; ++j)
			{
				if (GPIO_ReadInputData(PORT_COLS) & COLS[j])
				{
					while (GPIO_ReadInputData(PORT_COLS) & COLS[j]);
					GPIO_ResetBits(PORT_ROWS, ROWS[i]);
					return MATRIX[i][j];
				}
			}
			GPIO_ResetBits(PORT_ROWS, ROWS[i]);
		}
	}
}
