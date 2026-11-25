/*
 * SPI_Interface.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_SPI_SPI_INTERFACE_H_
#define MCAL_SPI_SPI_INTERFACE_H_

#include <stdint.h>
void SPI_MasterInit(void);
void SPI_SlaveInit(void);
uint8_t SPI_u8Transceive(uint8_t Copy_u8Data);

#endif /* MCAL_SPI_SPI_INTERFACE_H_ */
