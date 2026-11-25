/*
 * EEPROM_Program.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */


#include <stdint.h>
#include <util/delay.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"
#include "../../MCAL/TWI/TWI_Interface.h"

#include "EEPROM_Interface.h"
#include "EEPROM_Private.h"
#include "EEPROM_Config.h"

void EEPROM_voidInit(void)
{
    TWI_voidMasterInit(0);
}

void EEPROM_voidWriteByte(uint16_t Copy_u16Address, uint8_t Copy_u8Data)
{
    uint8_t Local_u8AddressPacket = (EEPROM_FIXED_ADDRESS) | (EEPROM_A2_PIN_CONNECTION << 2) | (uint8_t)(Copy_u16Address >> 8);

    TWI_u8SendStartCondition();

    TWI_u8SendSlaveAddressWithWrite(Local_u8AddressPacket);

    TWI_u8MasterWriteDataByte((uint8_t)Copy_u16Address);

    TWI_u8MasterWriteDataByte(Copy_u8Data);

    TWI_voidSendStopCondition();

    _delay_ms(10);
}

uint8_t EEPROM_u8ReadByte(uint16_t Copy_u16Address)
{
    uint8_t Local_u8Data = 0;

    uint8_t Local_u8AddressPacket = (EEPROM_FIXED_ADDRESS) | (EEPROM_A2_PIN_CONNECTION << 2) | (uint8_t)(Copy_u16Address >> 8);

    TWI_u8SendStartCondition();

    TWI_u8SendSlaveAddressWithWrite(Local_u8AddressPacket);

    TWI_u8MasterWriteDataByte((uint8_t)Copy_u16Address);

    TWI_u8SendRepeatedStart();

    TWI_u8SendSlaveAddressWithRead(Local_u8AddressPacket);

    TWI_u8MasterReadDataByte(&Local_u8Data);

    TWI_voidSendStopCondition();

    return Local_u8Data;
}
