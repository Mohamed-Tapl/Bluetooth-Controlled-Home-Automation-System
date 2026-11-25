/*
 * Buzzer_Private.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_BUZZER_BUZZER_PRIVATE_H_
#define HAL_BUZZER_BUZZER_PRIVATE_H_

#include "../../Common/Config.h"
#if Buzzer_Driver
    typedef enum
    {
        Buzzer_StateOff = 0,
        Buzzer_StateOn  = 1
    }Buzzer_State_t;
    typedef enum
    {
        Buzzer_PNPConnection =  0,
        Buzzer_NPNConnection =  1,
    }Buzzer_Connection_t;
#endif// Buzzer_Driver

#endif /* HAL_BUZZER_BUZZER_PRIVATE_H_ */
