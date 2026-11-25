/*
 * Relay_Program.c
 *
 *  Created on: Nov 23, 2026
 *      Author: Mohamed Tapl
 */

#include "../../Common/Config.h"

#if Relay_Driver

#include "Relay_Interface.h"

static uint8_t Relay_CurrentState = Relay_StateOff;

void Relay_Init(void)
{
    DIO_DirectionSelectForPin(RELAY_GROUP, RELAY_PIN, DIO_Output);
    /* Start off */
    Relay_Off();
}

void Relay_On(void)
{
    if(RELAY_ACTIVE_LEVEL == Relay_ActiveHigh)
    {
        DIO_WriteForPin(RELAY_GROUP, RELAY_PIN, DIO_High);
    }
    else
    {
        DIO_WriteForPin(RELAY_GROUP, RELAY_PIN, DIO_Low);
    }
    Relay_CurrentState = Relay_StateOn;
}

void Relay_Off(void)
{
    if(RELAY_ACTIVE_LEVEL == Relay_ActiveHigh)
    {
        DIO_WriteForPin(RELAY_GROUP, RELAY_PIN, DIO_Low);
    }
    else
    {
        DIO_WriteForPin(RELAY_GROUP, RELAY_PIN, DIO_High);
    }
    Relay_CurrentState = Relay_StateOff;
}

void Relay_Toggle(void)
{
    if(Relay_CurrentState == Relay_StateOff)
    {
        Relay_On();
    }
    else
    {
        Relay_Off();
    }
}

uint8_t Relay_GetState(void)
{
    return Relay_CurrentState;
}

#endif /* Relay_Driver */
