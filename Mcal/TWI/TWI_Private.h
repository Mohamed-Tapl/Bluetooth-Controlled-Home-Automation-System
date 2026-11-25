/*
 * TWI_Private.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TWI_TWI_PRIVATE_H_
#define MCAL_TWI_TWI_PRIVATE_H_

#include "../Atmega32Registers.h"

#define TWBR_Reg    _SetAddress8bit(0x20)
#define TWSR_Reg    _SetAddress8bit(0x21)
#define TWAR_Reg    _SetAddress8bit(0x22)
#define TWDR_Reg    _SetAddress8bit(0x23)
#define TWCR_Reg    _SetAddress8bit(0x56)

#define TWCR_TWINT  7
#define TWCR_TWEA   6
#define TWCR_TWSTA  5
#define TWCR_TWSTO  4
#define TWCR_TWWC   3
#define TWCR_TWEN   2
#define TWCR_TWIE   0

#define TWAR_TWGCE  0

#define TWSR_TWPS0  0
#define TWSR_TWPS1  1

#define TWI_STATUS_MASK                 0xF8
#define TWI_START_CONDITION_ACK         0x08
#define TWI_REPEATED_START_ACK          0x10
#define TWI_SLA_W_ACK                   0x18
#define TWI_SLA_R_ACK                   0x40
#define TWI_DATA_MT_ACK                 0x28
#define TWI_DATA_MR_ACK                 0x50

#define TWI_PRESCALER_1                 0
#define TWI_PRESCALER_4                 1
#define TWI_PRESCALER_16                2
#define TWI_PRESCALER_64                3

#define TWI_ACK_DISABLE                 0
#define TWI_ACK_ENABLE                  1

#define TWI_GC_DISABLE                  0
#define TWI_GC_ENABLE                   1

#endif /* MCAL_TWI_TWI_PRIVATE_H_ */
