/*
 * GIE_Program.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */



#include "GIE_Interface.h"

void GIE_Enable()
{
    SetBit(SREG_Reg,7);
}

void GIE_Disable()
{
    ClearBit(SREG_Reg,7);
}
