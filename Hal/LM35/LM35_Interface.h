/*
 * LM35_Interface.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_LM35_LM35_INTERFACE_H_
#define HAL_LM35_LM35_INTERFACE_H_

#include <stdint.h>
void LM35_Init(void);
void LM35_GetTemperature(uint8_t Copy_u8Channel, uint8_t *Temp_Value);

#endif /* HAL_LM35_LM35_INTERFACE_H_ */
