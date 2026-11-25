/*
 * Btn_Private.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef COMMON_BTN_PRIVATE_H_
#define COMMON_BTN_PRIVATE_H_


#include "../../Common/Config.h"
#if Button_Driver
typedef enum
{
    Btn_InternalPullup,
    Btn_ExternalPullup,
    Btn_ExternalPullDown,
}Btn_Connection_t;
typedef enum
{
    PullUp_Pressed,
    PullUp_NotPressed,
    PullDown_NotPressed=0,
    PullDown_Pressed,
}Btn_State_t;
#endif// Button_Driver

#endif /* COMMON_BTN_PRIVATE_H_ */
