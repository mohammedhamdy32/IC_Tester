/*
 * keypad.c
 *
 *  Created on: Oct 10, 2022
 *      Author: mohammedhamdy32
 */

#include "keypad.h"
#include "common_macros.h"
#include "std_types.h"
#include "avr/delay.h"

uint8 KEYPAD_adjustKeyNumber(uint8 button_num);

uint8 KEYPAD_getPressedKey()
{
	/* Make the columns pins as input by static configuration */
	for(int i=0;i<KEYPAD_COLUMN_NUMB;i++)
    	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_COL_START_PIN_ID+i, INPUT);

	while(1)
	{
       _delay_ms(50);
		for(int row=0;row<KEYPAD_ROWS_NUMB;row++)
		{
			/*Make row pin as output and write 0 in it*/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_START_PIN_ID+row , OUTPUT);
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_START_PIN_ID+row , BUTTON_IS_PRESSED);

			/*check if the button is pressed on channel or not*/
			for(int col=0;col<KEYPAD_COLUMN_NUMB;col++)
			{
			 if(GPIO_readPin(KEYPAD_COL_PORT_ID , KEYPAD_COL_START_PIN_ID+col) == BUTTON_IS_PRESSED )
			 {
				GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_START_PIN_ID+row , INPUT);
				return KEYPAD_adjustKeyNumber((row*KEYPAD_COLUMN_NUMB)+col+1);
			 }

			}
            /*make this pin input to open another pin in row*/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_START_PIN_ID+row , INPUT);
		}
}
}


uint8 KEYPAD_adjustKeyNumber(uint8 button_num)
{
	uint8 adjustNum=0;
	switch(button_num)
	{
	case 1:
			adjustNum = BUTTON1;
			break;
	case 2:
			adjustNum = BUTTON2;
			break;
	case 3:
			adjustNum = BUTTON3;
			break;
	case 4:
			adjustNum = BUTTON4;
			break;
	case 5:
			adjustNum = BUTTON5;
			break;
	case 6:
			adjustNum = BUTTON6;
			break;
	case 7:
			adjustNum = BUTTON7;
			break;
	case 8:
			adjustNum = BUTTON8;
			break;
	case 9:
			adjustNum = BUTTON9;
			break;
	case 10:
			adjustNum = BUTTON10;
			break;
	case 11:
			adjustNum = BUTTON11;
			break;
	case 12:
			adjustNum = BUTTON12;
			break;
	case 13:
			adjustNum = BUTTON13;
			break;
	case 14:
			adjustNum = BUTTON14;
			break;
	case 15:
			adjustNum = BUTTON15;
			break;
	case 16:
			adjustNum = BUTTON16;
			break;
	}
	return adjustNum;
}

