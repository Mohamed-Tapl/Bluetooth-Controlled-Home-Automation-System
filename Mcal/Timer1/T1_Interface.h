/*
 * T1_Interface.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TIMER1_T1_INTERFACE_H_
#define MCAL_TIMER1_T1_INTERFACE_H_

#include <stdint.h>

void MT1_voidInit(void);
void MT1_voidSetPreload(uint16_t Copy_u16Preload);
void MT1_voidSetCompareMatchA(uint16_t Copy_u16CompareValue);
void MT1_voidDelayMs_OVF(uint32_t Copy_u32DelayMs);
void MT1_voidDelayMs_CTC(uint32_t Copy_u32DelayMs);

/* Fast PWM on OC1A (PD5) */
void MT1_voidFastPWMInit(void);
void MT1_voidSetDutyCycle(uint8_t Copy_u8DutyPercent);

#endif /* MCAL_TIMER1_T1_INTERFACE_H_ */
