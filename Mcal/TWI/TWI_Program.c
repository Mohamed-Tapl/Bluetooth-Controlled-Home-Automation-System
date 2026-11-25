/*
 * TWI_Program.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */


#include <stdint.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"

#include "TWI_Interface.h"
#include "TWI_Private.h"
#include "TWI_Config.h"

void TWI_voidMasterInit(uint8_t Copy_u8MasterAddress)
{
    TWBR_Reg = TWI_BIT_RATE;

#if TWI_PRESCALER == TWI_PRESCALER_1
    ClearBit(TWSR_Reg, TWSR_TWPS0);
    ClearBit(TWSR_Reg, TWSR_TWPS1);
#elif TWI_PRESCALER == TWI_PRESCALER_4
    SetBit(TWSR_Reg, TWSR_TWPS0);
    ClearBit(TWSR_Reg, TWSR_TWPS1);
#elif TWI_PRESCALER == TWI_PRESCALER_16
    ClearBit(TWSR_Reg, TWSR_TWPS0);
    SetBit(TWSR_Reg, TWSR_TWPS1);
#elif TWI_PRESCALER == TWI_PRESCALER_64
    SetBit(TWSR_Reg, TWSR_TWPS0);
    SetBit(TWSR_Reg, TWSR_TWPS1);
#endif

    if(Copy_u8MasterAddress != 0)
    {
        TWAR_Reg = (Copy_u8MasterAddress << 1) | (TWI_GENERAL_CALL);
    }

    SetBit(TWCR_Reg, TWCR_TWEN);
}

void TWI_voidSlaveInit(uint8_t Copy_u8SlaveAddress)
{
    TWAR_Reg = (Copy_u8SlaveAddress << 1) | (TWI_GENERAL_CALL);

#if TWI_ACK_MODE == TWI_ACK_ENABLE
    SetBit(TWCR_Reg, TWCR_TWEA);
#else
    ClearBit(TWCR_Reg, TWCR_TWEA);
#endif

    SetBit(TWCR_Reg, TWCR_TWEN);
}

TWI_ErrorStatus_t TWI_u8SendStartCondition(void)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWSTA) | (1 << TWCR_TWEN);

    while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

    if((TWSR_Reg & TWI_STATUS_MASK) != TWI_START_CONDITION_ACK)
    {
        Local_u8ErrorStatus = TWI_START_CONDITION_ERR;
    }

    return Local_u8ErrorStatus;
}

TWI_ErrorStatus_t TWI_u8SendRepeatedStart(void)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWSTA) | (1 << TWCR_TWEN);

    while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

    if((TWSR_Reg & TWI_STATUS_MASK) != TWI_REPEATED_START_ACK)
    {
        Local_u8ErrorStatus = TWI_REPEATED_START_ERR;
    }

    return Local_u8ErrorStatus;
}

TWI_ErrorStatus_t TWI_u8SendSlaveAddressWithWrite(uint8_t Copy_u8SlaveAddress)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    TWDR_Reg = (Copy_u8SlaveAddress << 1);
    ClearBit(TWDR_Reg, 0);

    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWEN);

    while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

    if((TWSR_Reg & TWI_STATUS_MASK) != TWI_SLA_W_ACK)
    {
        Local_u8ErrorStatus = TWI_SLA_W_ACK_ERR;
    }

    return Local_u8ErrorStatus;
}

TWI_ErrorStatus_t TWI_u8SendSlaveAddressWithRead(uint8_t Copy_u8SlaveAddress)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    TWDR_Reg = (Copy_u8SlaveAddress << 1);
    SetBit(TWDR_Reg, 0);

    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWEN);

    while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

    if((TWSR_Reg & TWI_STATUS_MASK) != TWI_SLA_R_ACK)
    {
        Local_u8ErrorStatus = TWI_SLA_R_ACK_ERR;
    }

    return Local_u8ErrorStatus;
}

TWI_ErrorStatus_t TWI_u8MasterWriteDataByte(uint8_t Copy_u8DataByte)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    TWDR_Reg = Copy_u8DataByte;

    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWEN);

    while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

    if((TWSR_Reg & TWI_STATUS_MASK) != TWI_DATA_MT_ACK)
    {
        Local_u8ErrorStatus = TWI_DATA_MT_ACK_ERR;
    }

    return Local_u8ErrorStatus;
}

TWI_ErrorStatus_t TWI_u8MasterReadDataByte(uint8_t *Copy_pu8DataByte)
{
    TWI_ErrorStatus_t Local_u8ErrorStatus = TWI_OK;

    if(Copy_pu8DataByte != NULL)
    {
        TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1 << TWCR_TWEA);

        while(ReadBit(TWCR_Reg, TWCR_TWINT) == 0);

        if((TWSR_Reg & TWI_STATUS_MASK) != TWI_DATA_MR_ACK)
        {
            Local_u8ErrorStatus = TWI_DATA_MR_ACK_ERR;
        }
        else
        {
            *Copy_pu8DataByte = TWDR_Reg;
        }
    }
    else
    {
        Local_u8ErrorStatus = TWI_DATA_MR_NACK_ERR;
    }

    return Local_u8ErrorStatus;
}

void TWI_voidSendStopCondition(void)
{
    TWCR_Reg = (1 << TWCR_TWINT) | (1 << TWCR_TWSTO) | (1 << TWCR_TWEN);
}
