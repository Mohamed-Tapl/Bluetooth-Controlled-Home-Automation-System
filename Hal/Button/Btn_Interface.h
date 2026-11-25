/*
 * Btn_Interface.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_BUTTON_BTN_INTERFACE_H_
#define HAL_BUTTON_BTN_INTERFACE_H_

#include "../../Common/Config.h"

#if Button_Driver
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"

#include "../../Mcal/DIO/DIO_Interface.h"
#include "Btn_Private.h"
#include "Btn_Config.h"

void BTN_Init(uint8_t BtnGroup,uint8_t BtnPin,uint8_t BtnConnection);
uint8_t BTN_GetState(uint8_t BtnGroup,uint8_t BtnPin,uint8_t BtnConnection);

#endif// Button_Driver


#endif /* HAL_BUTTON_BTN_INTERFACE_H_ */
