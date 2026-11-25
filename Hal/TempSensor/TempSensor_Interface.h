/*
 * TempSensor_Interface.h
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_TEMPSENSOR_TEMPSENSOR_INTERFACE_H_
#define HAL_TEMPSENSOR_TEMPSENSOR_INTERFACE_H_

#include <stdint.h>
void TempSensor_Init(void);
void TempSensor_GetTemperature(uint8_t Copy_u8Channel, uint8_t *Temp_Value);

#endif /* HAL_TEMPSENSOR_TEMPSENSOR_INTERFACE_H_ */
