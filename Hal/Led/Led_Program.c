/*
 * Led_Program.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Fixed: removed hardcoded DIO_GroupB/DIO_Pin3 writes from Led_On().
 */


#include "Led_Interface.h"

void Led_Init(uint8_t LedGroup, uint8_t LedPin)
{
    DIO_DirectionSelectForPin(LedGroup, LedPin, DIO_Output);
}

void Led_On(uint8_t LedGroup, uint8_t LedPin, uint8_t ConnectionType)
{
    if(ConnectionType == Led_SourceConnection)
    {
        DIO_WriteForPin(LedGroup, LedPin, DIO_High);
    }
    else if(ConnectionType == Led_SinkConnection)
    {
        DIO_WriteForPin(LedGroup, LedPin, DIO_Low);
    }
    else
    {
        /* Invalid connection type */
    }
}

void Led_Off(uint8_t LedGroup, uint8_t LedPin, uint8_t ConnectionType)
{
    if(ConnectionType == Led_SourceConnection)
    {
        DIO_WriteForPin(LedGroup, LedPin, DIO_Low);
    }
    else if(ConnectionType == Led_SinkConnection)
    {
        DIO_WriteForPin(LedGroup, LedPin, DIO_High);
    }
    else
    {
        /* Invalid connection type */
    }
}

void Led_Toggle(uint8_t LedGroup, uint8_t LedPin)
{
    DIO_ToggleForPin(LedGroup, LedPin);
}
