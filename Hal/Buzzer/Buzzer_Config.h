/*
 * Bizzer_Config.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_BUZZER_BUZZER_CONFIG_H_
#define HAL_BUZZER_BUZZER_CONFIG_H_


#include "../../Common/Config.h"

#if Buzzer_Driver

#include "../../Mcal/DIO/DIO_Interface.h"

#define Buzzer_ConnectionType Buzzer_NPNConnection
#define BuzzerGroup           DIO_GroupA
#define BuzzerPin             DIO_Pin3

#endif// Buzzer_Driver


#endif /* HAL_BUZZER_BUZZER_CONFIG_H_ */
