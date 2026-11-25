/*
 * T1_Program.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */


#include "T1_Private.h"
#include "T1_Config.h"
#include "T1_Interface.h"
#include "../../Common/BitMath.h"
#include "../../Mcal/DIO/DIO_Interface.h"

void MT1_voidInit(void) {
    TCCR1A = 0x00;
    TCCR1B = 0x00;
}

void MT1_voidSetPreload(uint16_t Copy_u16Preload) {
    TCNT1 = Copy_u16Preload;
}

void MT1_voidSetCompareMatchA(uint16_t Copy_u16CompareValue) {
    OCR1A = Copy_u16CompareValue;
}

void MT1_voidDelayMs_OVF(uint32_t Copy_u32DelayMs) {
    uint32_t Local_u32Count = Copy_u32DelayMs;
    TCCR1A = 0x00;
    TCCR1B = 0x03;
    while(Local_u32Count > 0) {
        TCNT1 = 65411;
        while((TIFR & (1 << 2)) == 0);
        TIFR |= (1 << 2);
        Local_u32Count--;
    }
    TCCR1B = 0x00;
}

void MT1_voidDelayMs_CTC(uint32_t Copy_u32DelayMs) {
    uint32_t Local_u32Count = Copy_u32DelayMs;
    TCCR1A = 0x00;
    TCCR1B = 0x0B;
    OCR1A = 124;
    TCNT1 = 0;
    while(Local_u32Count > 0) {
        while((TIFR & (1 << 4)) == 0);
        TIFR |= (1 << 4);
        Local_u32Count--;
    }
    TCCR1B = 0x00;
}

/*------------ Fast PWM Mode 14: ICR1 = TOP, OC1A non-inverting ------------*/

void MT1_voidFastPWMInit(void)
{
    /* Set OC1A (PD5) as output for the PWM waveform */
    DIO_DirectionSelectForPin(DIO_GroupD, DIO_Pin5, DIO_Output);

    /*
     * Mode 14 — Fast PWM, ICR1 = TOP
     *   WGM13:10 = 1110  =>  WGM13=1 WGM12=1 WGM11=1 WGM10=0
     * OC1A: non-inverting (COM1A1=1, COM1A0=0)
     */
    TCCR1A = (1 << TCCR1A_COM1A1) | (1 << TCCR1A_WGM11);
    /* WGM13 + WGM12 + prescaler bits */
    TCCR1B = (1 << TCCR1B_WGM13) | (1 << TCCR1B_WGM12) | T1_PRESCALER;

    /* Set TOP value */
    ICR1 = T1_PWM_TOP;

    /* Start with 0% duty */
    OCR1A = 0;
}

void MT1_voidSetDutyCycle(uint8_t Copy_u8DutyPercent)
{
    if(Copy_u8DutyPercent > 100)
    {
        Copy_u8DutyPercent = 100;
    }
    /* OCR1A = TOP * DutyPercent / 100 */
    OCR1A = (uint16_t)(((uint32_t)T1_PWM_TOP * Copy_u8DutyPercent) / 100);
}
