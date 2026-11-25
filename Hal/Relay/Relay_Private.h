/*
 * Relay_Private.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_RELAY_RELAY_PRIVATE_H_
#define HAL_RELAY_RELAY_PRIVATE_H_

#include "../../Common/Config.h"

#if Relay_Driver

typedef enum
{
    Relay_ActiveHigh = 0,
    Relay_ActiveLow  = 1,
} Relay_ActiveLevel_t;

typedef enum
{
    Relay_StateOff = 0,
    Relay_StateOn  = 1,
} Relay_State_t;

#endif /* Relay_Driver */

#endif /* HAL_RELAY_RELAY_PRIVATE_H_ */
