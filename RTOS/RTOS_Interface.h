/*
 * RTOS_Interface.h
 *
 *  Created on: Nov 12, 2026
 *      Author: Mohamed Tapl
 */

#ifndef RTOS_RTOS_INTERFACE_H_
#define RTOS_RTOS_INTERFACE_H_

#include <stdint.h>

void RTOS_voidStart(void);
uint8_t RTOS_u8CreateTask(uint8_t Copy_u8Priority, uint16_t Copy_u16Periodicity, uint16_t Copy_u16FirstDelay, void (*Copy_pvTaskFunc)(void));
void RTOS_voidSuspendTask(uint8_t Copy_u8Priority);
void RTOS_voidResumeTask(uint8_t Copy_u8Priority);
void RTOS_voidDeleteTask(uint8_t Copy_u8Priority);


#endif /* RTOS_RTOS_INTERFACE_H_ */
