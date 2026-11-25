/*
 * T1_Config.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TIMER1_T1_CONFIG_H_
#define MCAL_TIMER1_T1_CONFIG_H_

/* Select: T1_NORMAL_MODE, T1_CTC_MODE, or T1_FAST_PWM */
#define T1_MODE         T1_FAST_PWM
#define T1_PRESCALER    T1_PRESCALER_8

/*
 * Fast-PWM ICR1 TOP value.
 * With F_CPU = 8 MHz, prescaler = 8:
 *   f_PWM = 8000000 / (8 * (TOP+1))
 *   TOP = 999  =>  f_PWM = 1000 Hz (audibly-quiet for fan control).
 */
#define T1_PWM_TOP      999

#endif /* MCAL_TIMER1_T1_CONFIG_H_ */
