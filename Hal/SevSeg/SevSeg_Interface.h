/*
 * SevSeg_Interface.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_SEVSEG_SEVSEG_INTERFACE_H_
#define HAL_SEVSEG_SEVSEG_INTERFACE_H_


#include <stdint.h>
#include "SevSeg_Private.h"
#include "SevSeg_Config.h"

void SSD_Init(uint8_t SSD_ID);

void SSD_DisplayNo(uint8_t SSD_ID , uint8_t Number);

#endif /* HAL_SEVSEG_SEVSEG_INTERFACE_H_ */
