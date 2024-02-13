/*
 * main.c
 *
 *  Created on: Feb 24, 2023
 *      Author: mohammedhamdy32
 */

#include "application.h"
#include "keypad.h"
#include "lcd.h"
#include "avr/io.h"
#include "avr/delay.h"
#include "external_eeprom.h"
#include "twi.h"
#include "common_macros.h"

int main()
{

	LCD_init();
	TWI_ConfigType twi_conf = {0b00000010,400000};
	TWI_init(&twi_conf); 


	GPIO_writePort(PORTC_ID, 0XF0);
	/*Intro of the device*/
	LCD_clearScreen();
	LCD_moveCursor(0,2);
	LCD_writeString("Wellcom to ");
	LCD_moveCursor(1,3);
	LCD_writeString("IC tester");
	//	_delay_ms(1000);
	LCD_clearScreen();




	uint8 pressed_key;

	//	pressed_key = search_IC_name(4600,&pressed_key,&pressed_key);
	//
	//	LCD_init();
	//	LCD_integerToString(pressed_key);
	//	_delay_ms(1000);
	//	LCD_clearScreen();

	//			test_the_ic(0b00010010100100,0b11111110111111,14); /*FIRST TEST for 7408*/
	//          0b11001000010010
	//			test_the_ic(0b00010010100100,0b10100100001001,14); /*third test*/
	//			test_the_ic(0b00010010100100,0b10000000000000,14); /*forth test test*/


//		EEPROM_writeByte(0x07, 0x24);
//		_delay_ms(10);
//		EEPROM_writeByte(1, 0x02);


	while(1)
	{
		LCD_writeString("+ : Testing");  /*to test an IC*/
		LCD_moveCursor(1,0);
		LCD_writeString("- : Add new test"); /*to add new IC*/

		pressed_key = KEYPAD_getPressedKey();
		_delay_ms(DELAY_BETWEEN_CLICKS_IN_MS);



		/********************** Testing the IC *************************/
		if( '+' == pressed_key )
		{ /*testing an IC*/

			LCD_clearScreen();
			LCD_writeString("+ : Enter number");
			LCD_moveCursor(1,0);
			LCD_writeString("- : Auto test");


			pressed_key = KEYPAD_getPressedKey();

			/***************** The user will enter the IC number **********************/
			if( '+' == pressed_key )
			{

				LCD_clearScreen();
				LCD_writeString("Enter IC number");

				/*Take the number from user*/
				uint8  IC_number[MAX_IC_NAME_SIZE+1]; /*The maximum size of name*/
				uint8 size;

				size = take_input_from_keypad(IC_number); /*The entered IC number*/

				if( size > MAX_IC_NAME_SIZE || size == 0 )
				{ /*if the size is greater than the max size, so I will display an error massage and continue*/
					max_size_error_display(MAX_IC_NAME_SIZE);
					continue;
				}

				/*Shifting the number to be like 007408*/
				sint8 j=MAX_IC_NAME_SIZE-1;
				for(sint8 i=size-1 ;i>=0 ; i--,j--)
				{
					IC_number[j] = IC_number[i];

				}
				/*put zeros in the empty places*/
				for(int i=0;i<MAX_IC_NAME_SIZE-size;i++)
					IC_number[i] = 0;



				/*Now, I have the IC number, so I will search it in EEPROM*/

				/*************************************************
				 *  search this IC number in EEPROM and
				 *  fill those variables pins_number,test_casess_number
				 *  io_map and test_case
				 * ************************************************/
				uint8  pins_number        =  16;
				uint8  test_casess_number =  1;      //0b00010010100100
				uint32 io_map             =  0;
				uint32 test_case =0 ;
				uint16 first_IC_address ;


			   uint32 IC_number_in_BCD = change_array_to_BCD(IC_number,MAX_IC_NAME_SIZE); /*contains the IC number in BCD form*/

				first_IC_address   = search_IC_name(IC_number_in_BCD , &test_casess_number , &io_map);
//				io_map             =  0b1001000000100100;  //0b00010010100100
				uint16 test_case_address = first_IC_address + 7 ; /*Add 7 to points on first test case*/

				if( first_IC_address == -1)/*The IC is not found*/
				{
					LCD_clearScreen();
					LCD_writeString("Not found the IC");
					_delay_ms(1000);
					LCD_clearScreen();
					continue;
				}


				/*If I found the IC, so let's test all cases */
				uint8 is_true;  /* 0 for false, 1 for true */
				uint8 i; /*number of test*/
				uint32 temp;
				for(i=0;i<test_casess_number;i++)
				{
					/*First get the test case*/
					temp = 0;
					test_case = 0;
					EEPROM_readByte(++test_case_address , &test_case );
					_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
					test_case = test_case << 8 ;
					EEPROM_readByte(test_case_address-1, &test_case);
					//					test_case = (test_case & (0x00FF) ) | (temp<<(pins_number/2) );
					test_case_address++;


					/*Then test this case*/
					is_true = test_the_ic(io_map,test_case,pins_number);

					/*check if this test case is true or not*/
					if(is_true == 0)
					{ /*so it's false*/
						break;
					}
					_delay_ms(DELAY_BETWEEN_TWO_TESTS__IN_MS);

				}

				/*if the IC not found display a massage*/


				if(is_true==0)
				{
					LCD_clearScreen();
					LCD_writeString("Failed at case ");
					LCD_moveCursor(1,0);
					LCD_integerToString(i+1); /*number of failed test*/
					LCD_writeString("  :(");
					KEYPAD_getPressedKey(); /*To wait till the user press any button*/
				}else
				{
					LCD_clearScreen();
					LCD_writeString("The test passes");
					LCD_moveCursor(1,0);
					LCD_writeString(" :) ");
					KEYPAD_getPressedKey(); /*To wait till the user press any button*/
				}




				//			test_the_ic(0b00010010100100,0b11111110111111,14); /*FIRST TEST for 7408*/
				//			test_the_ic(0b00010010100100,0b11001000010010,14); /*second test*/
				//			test_the_ic(0b00010010100100,0b10100100001001,14); /*third test*/
				//			test_the_ic(0b00010010100100,0b10000000000000,14); /*forth test test*/




				LCD_clearScreen(); /*clear at the end*/

			}else if( '-' == pressed_key ) /*Auto test, It will take much time*/
			{

			}

		}
		/**************** Adding new IC test *********************/
		else if('-' == pressed_key)
		{ /*Adding new IC*/
			LCD_clearScreen();
			LCD_writeString("+ : manual");
			LCD_moveCursor(1,0);
			LCD_writeString("- : By our program"); /*advanced step, which will add by a program on PC*/

			pressed_key = KEYPAD_getPressedKey();
			_delay_ms(DELAY_BETWEEN_CLICKS_IN_MS);


			/************The user will enter the IC number*************/
			if('+' == pressed_key)
			{ /*Adding IC manual by KEYPAD*/
				LCD_clearScreen();
				LCD_writeString("Pins number:");

				/*Take the number from user*/
				uint8 pins_number_arr[3]; /*The maximum size of name*/
				uint8 pins_arr_size; /*The number of digits of the array*/
				uint8 pins_number; /*The real number of pins*/

				pins_arr_size = take_input_from_keypad(pins_number_arr);


				/*check if the user enters a number of 0 or greater than 2 digits */
				if(pins_arr_size > 2 || pins_arr_size == 0  )
				{
					max_size_error_display(MAX_PINS_FOR_IC);
					continue;
				}

				if(pins_arr_size == 1) /*So the pins number is one digits only like, 8 pins */
				{
					pins_number = pins_number_arr[0];
				}else /*So, the pins number is two digits */
				{
					pins_number = pins_number_arr[1]; /*take the first digit*/
					pins_number = pins_number + pins_number_arr[0]*10; /*Add second digit by multiplying it with 10 */
				}


				LCD_clearScreen();

				/*check the pins number*/
				if(pins_number > MAX_PINS_FOR_IC)
				{
					max_size_error_display(MAX_PINS_FOR_IC);
					continue;
				}


				/*I took pins number from user, then I will take the IC number*/
				LCD_clearScreen();
				LCD_writeString("Test number :");

				uint8 IC_number[MAX_IC_NAME_SIZE];
				uint8 IC_number_size;

				IC_number_size = take_input_from_keypad(IC_number);

				/*If any error in entering the IC name*/
				if(IC_number_size > MAX_IC_NAME_SIZE || IC_number_size ==0)
				{
					max_size_error_display(MAX_IC_NAME_SIZE);
					continue;
				}

				/*Shifting the number to be like 007408*/
				sint8 j=MAX_IC_NAME_SIZE-1;
				for(sint8 i=IC_number_size-1 ;i>=0 ; i--,j--)
				{
					IC_number[j] = IC_number[i];

				}
				/*put zeros in the empty places*/
				for(int i=0;i<MAX_IC_NAME_SIZE-IC_number_size;i++)
					IC_number[i] = 0;



				/******************save this information in EEPROM*********************/

				/*First find the last avaible place in eemprom, so I will read first two bytes*/
				//	EEPROM_writeByte(AVIRABLE_ADDRESS_MSBs, 0);
				//	_delay_ms(10);
				//	EEPROM_writeByte(AVIRABLE_ADDRESS_LSBs, 1);
				//
				//	_delay_ms(10);


				/*** Get the available place from EEPROM ***/
				uint16 avi_place; /*The last available place in memeory to add the new IC in it*/
				EEPROM_readByte(AVIRABLE_ADDRESS_MSBs, &avi_place); /*Save the MSBs of the last available place*/
				avi_place = avi_place << 8;  /*Shift left to save the LSBs of the available palce */
				_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
				EEPROM_readByte(AVIRABLE_ADDRESS_LSBs , &avi_place); /*Save the LSBs of the last available place*/

				/*Add IC number in EEPROM*/
				for(uint8 i=0,j=0 ; i<MAX_IC_NAME_SIZE ; i+=2,j++)
				{
					/*Each number will be saved in BCD form so each two numbers will take 1 byte*/
					EEPROM_writeByte(avi_place+j , (IC_number[i+1]) | (IC_number[i]<<4) );
					_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
				}

				avi_place = avi_place + 3; /*increment 3 for the IC name*/

				uint16 temp_avi_palce = avi_place; /*To save this position to put the jump address in it*/

				avi_place = avi_place + 2; /*This place before increment is reserved for the jump address*/





				/*** Now I have the IC name and number of IC pins, lets take the test cases ***/
				LCD_clearScreen();
				LCD_writeString("Now be careful!!");
				LCD_moveCursor(1,0);
				LCD_writeString("in test cases");
				KEYPAD_getPressedKey(); /*wait till the user press any key*/

				uint8  test_cases_counter=0;
				uint8  test_case_bit_number=0;
				uint8  test_case[MAX_PINS_FOR_IC]={0};
				uint8  row=0 , col =0;
				uint8  ch1 = 'i' , ch2 = 'o'; /*ch1 and ch2 are the character which will be displayed on LCD*/

				/*The first time will be for taking input output mapping*/
				LCD_clearScreen();
				LCD_writeString("Enter I/O map");
				LCD_moveCursor(1,0);
				LCD_writeString("0 : IN   1 : OUT");
				KEYPAD_getPressedKey(); /*wait till the user press any key*/
				LCD_clearScreen();

				while(1){

					if(test_cases_counter != 0) /*So, it's not the I/O time */
					{
						LCD_clearScreen();
						LCD_writeString("Enter test case");
						_delay_ms(1000);
						LCD_clearScreen();
					}

					while(1) /*Loop till break*/
					{
						pressed_key = KEYPAD_getPressedKey();
						_delay_ms(DELAY_BETWEEN_CLICKS_IN_MS);

						/*To adjust the LCD screen if it reaches 17 places*/
						if(col == 16)
						{
							row = 1;
							col = 0;
							LCD_moveCursor(row,col);
						}


						if(test_case_bit_number == pins_number && pressed_key != OK_BUTTON && pressed_key != REMOVING_BUTTON ) /*The user entered all the test cases */
							continue;

						if(pressed_key == 0) /*So, the user enters 0 */
						{
							LCD_writeChar(ch1);
							test_case[test_case_bit_number] = 0;
							test_case_bit_number++;
							col++;

						}else if(pressed_key == 1)/*so the user enters 1*/
						{
							LCD_writeChar(ch2);
							test_case[test_case_bit_number] = 1;
							test_case_bit_number++;
							col++;

						}
						else if(pressed_key == REMOVING_BUTTON)
						{
							/*I was in the first place in second row, so I should go to upper one*/
							if(col == 0 && row == 1)
							{/*I must go to upper row*/
								row = 0;
								col = 16; /*it will be decremented below*/
							}
							test_case_bit_number--;
							col--;
							LCD_moveCursor(row,col);
							LCD_writeChar(' ');
							LCD_moveCursor(row,col);
						}else if(pressed_key == OK_BUTTON)
						{
							/*The user pressed OK*/

							if(test_case_bit_number == pins_number) /*The counter must be equal to pins number*/
								break;
						}

					}/*while bracket*/



					/*************Add this test case in EEPROM*******************/

					/*check if the memory is full or not*/
					LCD_clearScreen();


					adjust_testcase_to_16_bit(test_case,pins_number); /*adjust the test case to 16 bit*/

					if(avi_place >= 0xFFFC) /*so the memory is full*/
					{
						LCD_clearScreen();
						LCD_writeString("No space in memory");
						break;
					}

					/*Save test case in EEPROM*/

					/*Change the test_case array to uint16 to be easy to save it in memory*/
					uint16 temp_test_case =0x0000;

					for(uint8 i=0 ; i<MAX_PINS_FOR_IC ; i++)
					{
						uint8 bit = test_case[MAX_PINS_FOR_IC-i-1];
						if(bit==1)
							SET_BIT(temp_test_case,i);
						else
							CLEAR_BIT(temp_test_case,i);
					}



					LCD_clearScreen();
					LCD_integerToString(temp_test_case);
					_delay_ms(500);
					KEYPAD_getPressedKey();




					EEPROM_writeByte(avi_place   , (uint8)(temp_test_case>>8) );
					_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
					EEPROM_writeByte(avi_place + 1  , (uint8)temp_test_case );

					avi_place = avi_place + 2; /*increase two bytes, because the test case takes two bytes*/



					if(test_cases_counter == 0) /*So, it the I/O time, So, I will not ask the user to to enter another test or not */
					{
						ch1 = '0';
						ch2 = '1';
						test_cases_counter ++;
						test_case_bit_number=0;
						row=0;
						col=0;
						continue;
					}


					/*Ask the user if he wants to add another test*/
					LCD_clearScreen();
					LCD_writeString("Add another test");
					LCD_moveCursor(1,0);
					LCD_writeString("+ : YES | - : NO");

					pressed_key = KEYPAD_getPressedKey();
					_delay_ms(DELAY_BETWEEN_CLICKS_IN_MS);

					if('+' == pressed_key)  /*The user wants to add another test case*/
					{
						test_cases_counter++;
						test_case_bit_number=0;
						row=0;
						col=0;
					}
					else if('-' == pressed_key) /*The user wants to exit*/
						break;

				}/*while bracket*/


				/**************Last step Save the available place****************/

				/*So, now I will save the Jump address in memory place saved in variable temp_avi_palce */
				uint16 Jump_palce; /*contains the address of the new Jump place*/
				Jump_palce = temp_avi_palce + 5 + 2*test_cases_counter -1; /* 5 = 3 for IO mapping, 2 for Jump address */
				EEPROM_writeByte(temp_avi_palce , Jump_palce>>8 ); /*Save MSBs */
				_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
				EEPROM_writeByte(temp_avi_palce+1 , (uint8)Jump_palce); /*Save LSBs*/

				/*The last step is to ask the user if he was sure that he wants to save this IC*/
				LCD_clearScreen();
				LCD_writeString("sure to save ?");
				LCD_moveCursor(1,0);
				LCD_writeString("Press OK if sure");
				pressed_key = KEYPAD_getPressedKey();

				if( OK_BUTTON == pressed_key)
				{ /*So now change the available place with the new palce*/
					EEPROM_writeByte(0 , Jump_palce>>8 ); /*Save MSBs */
					_delay_ms(DELAY_BETWEEN_TWO_EEPROM_RW);
					EEPROM_writeByte(1 , (uint8)Jump_palce); /*Save LSBs*/
				}








			}
			else if('-' == pressed_key) /*Not ready yet*/
			{
				uint8 x;
				EEPROM_writeByte(7,14);
				EEPROM_readByte(7,&x);
				LCD_clearScreen();
				LCD_integerToString(x);
				_delay_ms(1000);
			}


			LCD_clearScreen();/*clear screen at the end*/

		}/*ending of testing or adding ICs*/

		LCD_clearScreen();/*clear screen at the end*/


	}
}
