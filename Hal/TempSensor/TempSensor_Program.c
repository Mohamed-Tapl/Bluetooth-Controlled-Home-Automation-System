/*
 * TempSensor_Program.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Mohamed Tapl
 */

#include <stdint.h>
#include "../../Common/Definition.h"
#include "../../MCAL/ADC/ADC_Interface.h"
#include "TempSensor_Interface.h"
#include "TempSensor_Private.h"
#include "TempSensor_Config.h"

void TempSensor_Init(void)
{

}

void TempSensor_GetTemperature(uint8_t Copy_u8Channel, uint8_t *Temp_Value)
{
    if (Temp_Value != NULL)
    {
        uint16_t Local_u16ADCResult = 0;

        ADC_Read(Copy_u8Channel, &Local_u16ADCResult, TEMPSENSOR_TIMEOUT);

        *Temp_Value = (uint8_t)(((uint32_t)Local_u16ADCResult * TEMPSENSOR_VREF_MV) / (TEMPSENSOR_ADC_MAX_RES * 10));
    }
}
