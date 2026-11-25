/*
 * DcMotor_Interface.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Extended: DC_SetSpeed() for PWM fan-speed control via Timer1 OC1A.
 */

#ifndef HAL_DCMOTOR_DCMOTOR_INTERFACE_H_
#define HAL_DCMOTOR_DCMOTOR_INTERFACE_H_

#include "../../Mcal/DIO/DIO_Interface.h"
#include "DcMotor_Private.h"
#include "DcMotor_Config.h"

void DC_Init(Dc_config_t *Config);
/*API for On Off Control Only*/
void DC_On(Dc_config_t *Config);
void DC_Off(Dc_config_t *Config);
/*API for On Off&Direction Control */
void DC_OnCW(Dc_config_t *Config);
void DC_OnCCW(Dc_config_t *Config);

/*
 * Set motor speed via PWM duty cycle (0-100%).
 * Uses Timer1 Fast-PWM on OC1A/PD5 as the enable pin.
 */
void DC_SetSpeed(uint8_t Copy_u8SpeedPercent);

#endif /* HAL_DCMOTOR_DCMOTOR_INTERFACE_H_ */
