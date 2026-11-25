/*
 * Relay_Config.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Mohamed Tapl
 *
 *  Relay module on PB4 (via NPN/ULN2003 → active-high logic)
 */

#ifndef HAL_RELAY_RELAY_CONFIG_H_
#define HAL_RELAY_RELAY_CONFIG_H_

#include "../../Common/Config.h"

#if Relay_Driver

#include "../../Mcal/DIO/DIO_Interface.h"

#define RELAY_GROUP          DIO_GroupB
#define RELAY_PIN            DIO_Pin4
#define RELAY_ACTIVE_LEVEL   Relay_ActiveHigh

#endif /* Relay_Driver */

#endif /* HAL_RELAY_RELAY_CONFIG_H_ */
