/*
 * SPI_Program.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#include <stdint.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"
#include "SPI_Interface.h"
#include "SPI_Private.h"
#include "SPI_Config.h"

void SPI_MasterInit(void)
{
    SetBit(SPCR_Reg, SPCR_MSTR);

#if SPI_DATA_ORDER == SPI_DATA_LSB_FIRST
    SetBit(SPCR_Reg, SPCR_DORD);
#else
    ClearBit(SPCR_Reg, SPCR_DORD);
#endif

#if SPI_CLOCK_POLARITY == SPI_CPOL_HIGH
    SetBit(SPCR_Reg, SPCR_CPOL);
#else
    ClearBit(SPCR_Reg, SPCR_CPOL);
#endif

#if SPI_CLOCK_PHASE == SPI_CPHA_TRAILING
    SetBit(SPCR_Reg, SPCR_CPHA);
#else
    ClearBit(SPCR_Reg, SPCR_CPHA);
#endif

#if SPI_CLOCK_RATE == SPI_CLOCK_FOSC_4
    ClearBit(SPCR_Reg, SPCR_SPR1);
    ClearBit(SPCR_Reg, SPCR_SPR0);
    ClearBit(SPSR_Reg, SPSR_SPI2X);
#elif SPI_CLOCK_RATE == SPI_CLOCK_FOSC_16
    ClearBit(SPCR_Reg, SPCR_SPR1);
    SetBit(SPCR_Reg, SPCR_SPR0);
    ClearBit(SPSR_Reg, SPSR_SPI2X);
#elif SPI_CLOCK_RATE == SPI_CLOCK_FOSC_64
    SetBit(SPCR_Reg, SPCR_SPR1);
    ClearBit(SPCR_Reg, SPCR_SPR0);
    ClearBit(SPSR_Reg, SPSR_SPI2X);
#elif SPI_CLOCK_RATE == SPI_CLOCK_FOSC_128
    SetBit(SPCR_Reg, SPCR_SPR1);
    SetBit(SPCR_Reg, SPCR_SPR0);
    ClearBit(SPSR_Reg, SPSR_SPI2X);
#endif

    SetBit(SPCR_Reg, SPCR_SPE);
}

void SPI_SlaveInit(void)
{
    ClearBit(SPCR_Reg, SPCR_MSTR);

#if SPI_DATA_ORDER == SPI_DATA_LSB_FIRST
    SetBit(SPCR_Reg, SPCR_DORD);
#else
    ClearBit(SPCR_Reg, SPCR_DORD);
#endif

#if SPI_CLOCK_POLARITY == SPI_CPOL_HIGH
    SetBit(SPCR_Reg, SPCR_CPOL);
#else
    ClearBit(SPCR_Reg, SPCR_CPOL);
#endif

#if SPI_CLOCK_PHASE == SPI_CPHA_TRAILING
    SetBit(SPCR_Reg, SPCR_CPHA);
#else
    ClearBit(SPCR_Reg, SPCR_CPHA);
#endif

    SetBit(SPCR_Reg, SPCR_SPE);
}

uint8_t SPI_u8Transceive(uint8_t Copy_u8Data)
{
    SPDR_Reg = Copy_u8Data;

    while((ReadBit(SPSR_Reg, SPSR_SPIF)) == (0));

    return SPDR_Reg;
}
