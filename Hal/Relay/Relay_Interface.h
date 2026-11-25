/*
 * Relay_Interface.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_RELAY_RELAY_INTERFACE_H_
#define HAL_RELAY_RELAY_INTERFACE_H_

#include "../../Common/Config.h"

#if Relay_Driver

#include <stdint.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "Relay_Private.h"
#include "Relay_Config.h"

void Relay_Init(void);
void Relay_On(void);
void Relay_Off(void);
void Relay_Toggle(void);
uint8_t Relay_GetState(void);

#endif /* Relay_Driver */

#endif /* HAL_RELAY_RELAY_INTERFACE_H_ */
