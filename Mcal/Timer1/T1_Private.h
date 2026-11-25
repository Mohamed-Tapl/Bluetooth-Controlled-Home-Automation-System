/*
 * T1_Private.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_TIMER1_T1_PRIVATE_H_
#define MCAL_TIMER1_T1_PRIVATE_H_

/* Use central register file definitions where available, keep local
   aliases for backward compatibility with the existing MT1 delay functions. */
#define TCCR1A          *((volatile uint8_t*)0x4F)
#define TCCR1B          *((volatile uint8_t*)0x4E)
#define TCNT1           *((volatile uint16_t*)0x4C)
#define OCR1A           *((volatile uint16_t*)0x4A)
#define OCR1B           *((volatile uint16_t*)0x48)
#define ICR1            *((volatile uint16_t*)0x46)
#define TIMSK           *((volatile uint8_t*)0x59)
#define TIFR            *((volatile uint8_t*)0x58)

#define T1_NORMAL_MODE  0
#define T1_CTC_MODE     1
#define T1_FAST_PWM     2

#define T1_PRESCALER_STOP           0
#define T1_PRESCALER_1              1
#define T1_PRESCALER_8              2
#define T1_PRESCALER_64             3
#define T1_PRESCALER_256            4
#define T1_PRESCALER_1024           5

/* TCCR1A bit positions */
#define TCCR1A_WGM10    0
#define TCCR1A_WGM11    1
#define TCCR1A_FOC1B    2
#define TCCR1A_FOC1A    3
#define TCCR1A_COM1B0   4
#define TCCR1A_COM1B1   5
#define TCCR1A_COM1A0   6
#define TCCR1A_COM1A1   7

/* TCCR1B bit positions */
#define TCCR1B_CS10     0
#define TCCR1B_CS11     1
#define TCCR1B_CS12     2
#define TCCR1B_WGM12    3
#define TCCR1B_WGM13    4

#endif /* MCAL_TIMER1_T1_PRIVATE_H_ */
