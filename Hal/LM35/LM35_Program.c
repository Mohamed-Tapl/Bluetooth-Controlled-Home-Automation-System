/*
 * LM35_Program.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */


#include <stdint.h>
#include "../../Common/Definition.h"
#include "../../MCAL/ADC/ADC_Interface.h"
#include "LM35_Interface.h"
#include "LM35_Private.h"
#include "LM35_Config.h"

void LM35_Init(void)
{

}

void LM35_GetTemperature(uint8_t Copy_u8Channel, uint8_t *Temp_Value)
{
    if (Temp_Value != NULL)
    {
        uint16_t Local_u16ADCResult = 0;

        ADC_Read(Copy_u8Channel, &Local_u16ADCResult, LM35_TIMEOUT);

        *Temp_Value = (uint8_t)(((uint32_t)Local_u16ADCResult * LM35_VREF_MV) / (LM35_ADC_MAX_RES * 10));
    }
}
