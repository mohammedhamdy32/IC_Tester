/*
 * keypad.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mohammedhamdy32
 */

#include "gpio.h"

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define KEYPAD_ROWS_NUMB      4
#define KEYPAD_COLUMN_NUMB    4

#define BUTTON_IS_PRESSED     0

/*Row port and it's start pin*/
#define KEYPAD_ROW_PORT_ID          PORTA_ID
#define KEYPAD_ROW_START_PIN_ID     PIN0_ID

/*Column port and it's start pin*/
#define KEYPAD_COL_PORT_ID          PORTA_ID
#define KEYPAD_COL_START_PIN_ID     PIN4_ID


/*To set the value of buttons*/
#define BUTTON1          7
#define BUTTON2          8
#define BUTTON3          9
#define BUTTON4         '/'
#define BUTTON5          4
#define BUTTON6          5
#define BUTTON7          6
#define BUTTON8         '*'
#define BUTTON9          1
#define BUTTON10         2
#define BUTTON11         3
#define BUTTON12        '-'
#define BUTTON13        ' '
#define BUTTON14         0
#define BUTTON15        ' '
#define BUTTON16        '+'





uint8 KEYPAD_getPressedKey();


#endif /* KEYPAD_H_ */
