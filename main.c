/*
 * main.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Project One: Bluetooth-Controlled Home Automation System
 *  Layered AVR ATmega32 Architecture with Cooperative RTOS
 */

#include <stdint.h>
#include "Common/Config.h"
#include "Common/Definition.h"

#include "RTOS/RTOS_Interface.h"
#include "App/App.h"

int main(void)
{
    /* =====================================================================
     * 1. SYSTEM INITIALIZATION
     *    Configures all MCAL drivers (DIO, Timer0, Timer1, UART, TWI, GIE)
     *    and HAL drivers (LCD 4-bit, DC Motor PWM, Relay, LEDs, Buttons,
     *    Buzzer, External EEPROM 24C08).
     * ===================================================================== */
    App_voidInit();

    /* =====================================================================
     * 2. COOPERATIVE RTOS TASK CREATION (Timer0 base tick = 10 ms)
     *
     * Task 0: Bluetooth Command Parser (Priority 0)
     *         Period: 50 ms (5 ticks), First Delay: 0 ticks
     * Task 1: Manual Input Debounce & Override (Priority 1)
     *         Period: 50 ms (5 ticks), First Delay: 1 tick (load offset)
     * Task 2: Physical Actuator Control (Priority 2)
     *         Period: 100 ms (10 ticks), First Delay: 2 ticks
     * Task 3: 16x2 Character LCD Display (Priority 3)
     *         Period: 250 ms (25 ticks), First Delay: 5 ticks
     * Task 4: System Heartbeat & Monitoring (Priority 4)
     *         Period: 500 ms (50 ticks), First Delay: 10 ticks
     * Task 5: EEPROM Persistent Data Logger (Priority 5)
     *         Period: 1000 ms (100 ticks), First Delay: 20 ticks
     * ===================================================================== */
    RTOS_u8CreateTask(0, 5,   0,  &App_Task_Bluetooth);
    RTOS_u8CreateTask(1, 5,   1,  &App_Task_ManualInputs);
    RTOS_u8CreateTask(2, 10,  2,  &App_Task_ActuatorControl);
    RTOS_u8CreateTask(3, 25,  5,  &App_Task_LCDDisplay);
    RTOS_u8CreateTask(4, 50,  10, &App_Task_SystemMonitoring);
    RTOS_u8CreateTask(5, 100, 20, &App_Task_EEPROMManager);

    /* =====================================================================
     * 3. START RTOS SCHEDULER
     *    Enables Timer0 normal mode overflow interrupts and global interrupts.
     * ===================================================================== */
    RTOS_voidStart();

    /* =====================================================================
     * 4. BACKGROUND IDLE LOOP
     * ===================================================================== */
    while (1)
    {
        /* Tasks are executed periodically from the RTOS scheduler */
    }

    return 0;
}
