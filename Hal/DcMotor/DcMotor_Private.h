/*
 * DcMotor_Private.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef HAL_DCMOTOR_DCMOTOR_PRIVATE_H_
#define HAL_DCMOTOR_DCMOTOR_PRIVATE_H_


#include <stdint.h>


typedef enum
{
    Dc_WithoutHBridge,
    Dc_WithHBridge,
}Dc_ConnectionType_t;

typedef struct
{
    uint8_t ConnectionType;
    uint8_t Dc_M1Group;
    uint8_t Dc_M1Pin;
    uint8_t Dc_M2Group;
    uint8_t Dc_M2Pin;
}Dc_config_t;


#endif /* HAL_DCMOTOR_DCMOTOR_PRIVATE_H_ */
