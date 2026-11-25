/*
 * Buzzer_Interface.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_BUZZER_BUZZER_INTERFACE_H_
#define HAL_BUZZER_BUZZER_INTERFACE_H_

#include "../../Common/Config.h"
#if Buzzer_Driver

#include <stdint.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"
#include "../../Mcal/DIO/DIO_Interface.h"
#include "Buzzer_Private.h"
#include "Buzzer_Config.h"

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);

#endif// Buzzer_Driver

#endif /* HAL_BUZZER_BUZZER_INTERFACE_H_ */
