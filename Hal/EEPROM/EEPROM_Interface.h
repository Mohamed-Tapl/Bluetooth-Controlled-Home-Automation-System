/*
 * EEPROM_Interface.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_EEPROM_EEPROM_INTERFACE_H_
#define HAL_EEPROM_EEPROM_INTERFACE_H_

#include <stdint.h>

void EEPROM_voidInit(void);
void EEPROM_voidWriteByte(uint16_t Copy_u16Address, uint8_t Copy_u8Data);
uint8_t EEPROM_u8ReadByte(uint16_t Copy_u16Address);

#endif /* HAL_EEPROM_EEPROM_INTERFACE_H_ */
