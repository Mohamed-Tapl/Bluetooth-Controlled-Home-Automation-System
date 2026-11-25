/*
 * SPI_Private.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_SPI_SPI_PRIVATE_H_
#define MCAL_SPI_SPI_PRIVATE_H_

#include "../Atmega32Registers.h"

#define SPCR_Reg    _SetAddress8bit(0x2D)
#define SPSR_Reg    _SetAddress8bit(0x2E)
#define SPDR_Reg    _SetAddress8bit(0x2F)

#define SPCR_SPIE   7
#define SPCR_SPE    6
#define SPCR_DORD   5
#define SPCR_MSTR   4
#define SPCR_CPOL   3
#define SPCR_CPHA   2
#define SPCR_SPR1   1
#define SPCR_SPR0   0

#define SPSR_SPIF   7
#define SPSR_WCOL   6
#define SPSR_SPI2X  0

#define SPI_DATA_MSB_FIRST 0
#define SPI_DATA_LSB_FIRST 1

#define SPI_CPOL_LOW       0
#define SPI_CPOL_HIGH      1

#define SPI_CPHA_LEADING   0
#define SPI_CPHA_TRAILING  1

#define SPI_CLOCK_FOSC_4   0
#define SPI_CLOCK_FOSC_16  1
#define SPI_CLOCK_FOSC_64  2
#define SPI_CLOCK_FOSC_128 3


#endif /* MCAL_SPI_SPI_PRIVATE_H_ */
