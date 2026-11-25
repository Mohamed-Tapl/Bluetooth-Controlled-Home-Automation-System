/*
 * RTOS_Program.c
 *
 *  Created on: Nov 12, 2026
 *      Author: Mohamed Tapl
 */


#include <stdint.h>
#include "../Common/Definition.h"
#include "../Common/BitMath.h"

#include "../MCAL/Timer0/T0_Interface.h"
#include "../MCAL/GIE/GIE_Interface.h"

#include "RTOS_Interface.h"
#include "RTOS_Private.h"
#include "RTOS_Config.h"

static Task_t SystemTasks[RTOS_MAX_NUM_TASKS] = {0};

void RTOS_voidStart(void)
{
    T0_NormalCallBack(&RTOS_voidScheduler);
    T0_NormalInit();
    GIE_Enable();
}

uint8_t RTOS_u8CreateTask(uint8_t Copy_u8Priority, uint16_t Copy_u16Periodicity, uint16_t Copy_u16FirstDelay, void (*Copy_pvTaskFunc)(void))
{
    uint8_t Local_u8ErrorState = 0;

    if (Copy_u8Priority < RTOS_MAX_NUM_TASKS)
    {
        if (SystemTasks[Copy_u8Priority].State == TASK_EMPTY)
        {
            SystemTasks[Copy_u8Priority].Periodicity = Copy_u16Periodicity;
            SystemTasks[Copy_u8Priority].FirstDelay  = Copy_u16FirstDelay;
            SystemTasks[Copy_u8Priority].TaskFunc    = Copy_pvTaskFunc;
            SystemTasks[Copy_u8Priority].State       = TASK_READY;
            Local_u8ErrorState = 1;
        }
    }
    return Local_u8ErrorState;
}

void RTOS_voidSuspendTask(uint8_t Copy_u8Priority)
{
    if (Copy_u8Priority < RTOS_MAX_NUM_TASKS)
    {
        SystemTasks[Copy_u8Priority].State = TASK_SUSPENDED;
    }
}

void RTOS_voidResumeTask(uint8_t Copy_u8Priority)
{
    if (Copy_u8Priority < RTOS_MAX_NUM_TASKS)
    {
        SystemTasks[Copy_u8Priority].State = TASK_READY;
    }
}

void RTOS_voidDeleteTask(uint8_t Copy_u8Priority)
{
    if (Copy_u8Priority < RTOS_MAX_NUM_TASKS)
    {
        SystemTasks[Copy_u8Priority].State = TASK_EMPTY;
    }
}

static void RTOS_voidScheduler(void)
{
    uint8_t Local_u8TaskCounter;

    for (Local_u8TaskCounter = 0; Local_u8TaskCounter < RTOS_MAX_NUM_TASKS; Local_u8TaskCounter++)
    {
        if (SystemTasks[Local_u8TaskCounter].State == TASK_READY)
        {
            if (SystemTasks[Local_u8TaskCounter].FirstDelay == 0)
            {
                if (SystemTasks[Local_u8TaskCounter].TaskFunc != NULL)
                {
                    SystemTasks[Local_u8TaskCounter].TaskFunc();
                }
                SystemTasks[Local_u8TaskCounter].FirstDelay = SystemTasks[Local_u8TaskCounter].Periodicity - 1;
            }
            else
            {
                SystemTasks[Local_u8TaskCounter].FirstDelay--;
            }
        }
    }
}
