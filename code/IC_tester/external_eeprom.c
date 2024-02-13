/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"
#include "avr/delay.h"

uint8 EEPROM_writeByte(uint32 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte( (uint8)((0xA0) | ((u16addr & 0x00070000)>>15)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte( (uint8) (u16addr>>8)); /*Send the first byte from MSB*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte( (uint8) (u16addr));  /*Send the second byte from LSB*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;


	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readByte(uint32 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x00070000)>>15)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return 11;

	/* Send the required memory location address */
	TWI_writeByte( (uint8) (u16addr>>8)); /*Send the first byte from MSB*/

	TWI_writeByte( (uint8) (u16addr));  /*Send the second byte from LSB*/

	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return 12;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return 13;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte( (uint8)( (0xA0) |  ( (u16addr & 0x00070000)>>15) | 1 ) );
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return 14;

	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return 15;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}
