/*
 * TWI_Interface.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TWI_TWI_INTERFACE_H_
#define MCAL_TWI_TWI_INTERFACE_H_

#include <stdint.h>

typedef enum
{
    TWI_START_CONDITION_ERR = 0,
    TWI_REPEATED_START_ERR,
    TWI_SLA_W_ACK_ERR,
    TWI_SLA_W_NACK_ERR,
    TWI_SLA_R_ACK_ERR,
    TWI_SLA_R_NACK_ERR,
    TWI_DATA_MT_ACK_ERR,
    TWI_DATA_MT_NACK_ERR,
    TWI_DATA_MR_ACK_ERR,
    TWI_DATA_MR_NACK_ERR,
    TWI_OK
} TWI_ErrorStatus_t;

void TWI_voidMasterInit(uint8_t Copy_u8MasterAddress);
void TWI_voidSlaveInit(uint8_t Copy_u8SlaveAddress);

TWI_ErrorStatus_t TWI_u8SendStartCondition(void);
TWI_ErrorStatus_t TWI_u8SendRepeatedStart(void);

TWI_ErrorStatus_t TWI_u8SendSlaveAddressWithWrite(uint8_t Copy_u8SlaveAddress);
TWI_ErrorStatus_t TWI_u8SendSlaveAddressWithRead(uint8_t Copy_u8SlaveAddress);

TWI_ErrorStatus_t TWI_u8MasterWriteDataByte(uint8_t Copy_u8DataByte);
TWI_ErrorStatus_t TWI_u8MasterReadDataByte(uint8_t *Copy_pu8DataByte);

void TWI_voidSendStopCondition(void);

#endif /* MCAL_TWI_TWI_INTERFACE_H_ */
