/*
 * T0_Config.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TIMER0_T0_CONFIG_H_
#define MCAL_TIMER0_T0_CONFIG_H_

#if Timer0_Driver

#define T0_ClockSelect           T0_Prescaller_64

#if T0_Normal
/*
 * Dedicated RTOS Tick Configuration (F_CPU = 8 MHz):
 * Prescaler = 64  =>  Timer0 tick = 8 us (125 kHz).
 * Counts per overflow = 256 - 6 = 250 counts = 2.0 ms.
 * Number of overflows = 5.
 * Base RTOS tick period = 5 * 2.0 ms = 10.0 ms.
 */
#define T0_InitPreloadValue      6
#define T0_NoOfOVFCount          5
#endif// _MCAL_TIMER0_T0_CONFIG_H


#if T0_CTC

#endif// _MCAL_TIMER0_T0_CONFIG_H




#if T0_PWM
    /*
     //! 1- T0_PWMFast
     //! 2- T0_PWMPhase
     */
    #define T0_PWMMode      T0_PWMFast
        /*
     //! 1- OC0_NonInverting
     //! 2- OC0_Inverting
     */
    #define T0_PWMAction    OC0_NonInverting
#endif// _MCAL_TIMER0_T0_CONFIG_H




#endif// Timer0_Driver


#endif /* MCAL_TIMER0_T0_CONFIG_H_ */
