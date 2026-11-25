/*
 * Buzzer_Program.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */



#include "../../Common/Config.h"
#if Buzzer_Driver

#include "Buzzer_Interface.h"


void Buzzer_Init(void)
{
    DIO_DirectionSelectForPin(BuzzerGroup, BuzzerPin, DIO_Output);
}
void Buzzer_On(void)
{
    uint8_t OutputState = (Buzzer_ConnectionType == Buzzer_NPNConnection)
        ? DIO_High
        : DIO_Low;
    DIO_WriteForPin(BuzzerGroup, BuzzerPin, OutputState);
}
void Buzzer_Off(void)
{
    uint8_t OutputState = (Buzzer_ConnectionType == Buzzer_NPNConnection)
        ? DIO_Low
        : DIO_High;
    DIO_WriteForPin(BuzzerGroup, BuzzerPin, OutputState);
}
void Buzzer_Toggle(void)
{
    DIO_ToggleForPin(BuzzerGroup, BuzzerPin);
}




#endif// Buzzer_Driver
