/*
 * RTOS_Private.h
 *
 *  Created on: Nov 12, 2026
 *      Author: Mohamed Tapl
 */

#ifndef RTOS_RTOS_PRIVATE_H_
#define RTOS_RTOS_PRIVATE_H_


typedef struct
{
    uint16_t Periodicity;
    uint16_t FirstDelay;
    void (*TaskFunc)(void);
    uint8_t State;
} Task_t;

#define TASK_EMPTY       0
#define TASK_READY       1
#define TASK_SUSPENDED   2

static void RTOS_voidScheduler(void);

#endif /* RTOS_RTOS_PRIVATE_H_ */
