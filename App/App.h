/*
 * App.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Project One: Bluetooth-Controlled Home Automation System
 *  Public interface for application initialization, state machine,
 *  and cooperative RTOS task entry points.
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include <stdint.h>
#include "App_Config.h"

/* =========================================================================
 *                         SHARED SYSTEM STATE TYPES
 * ========================================================================= */
typedef enum
{
    APP_STATE_OFF = 0,
    APP_STATE_ON  = 1
} App_DeviceState_t;

typedef enum
{
    APP_MODE_BLUETOOTH = 0,
    APP_MODE_MANUAL_OVERRIDE = 1
} App_ControlMode_t;

typedef struct
{
    uint8_t  Light1_State;      /* APP_STATE_OFF / APP_STATE_ON */
    uint8_t  Light2_State;      /* APP_STATE_OFF / APP_STATE_ON */
    uint8_t  Relay_State;       /* APP_STATE_OFF / APP_STATE_ON */
    uint8_t  Fan_Speed;         /* 0, 35, 70, 100 */
    uint8_t  ControlMode;       /* APP_MODE_BLUETOOTH / APP_MODE_MANUAL_OVERRIDE */
    uint32_t UptimeSeconds;     /* Incremented once per second */
    uint8_t  LogCount;          /* Stored log records count in external EEPROM */
} App_SystemState_t;

/* Log record entry stored in external EEPROM */
typedef struct
{
    uint8_t  EventId;
    uint8_t  DeviceId;
    uint8_t  NewState;
    uint16_t TimestampSec;
} App_LogRecord_t;

/* =========================================================================
 *                         PUBLIC API DECLARATIONS
 * ========================================================================= */

/**
 * @brief  Initializes all MCAL and HAL peripherals required for the
 *         Bluetooth-Controlled Home Automation System.
 */
void App_voidInit(void);

/**
 * @brief  Returns a read-only pointer to current system state snapshot.
 */
const App_SystemState_t* App_psysGetState(void);

/**
 * @brief  Requests non-blocking buzzer sound feedback.
 * @param  Copy_u8BeepDurationTicks: duration in 50ms ticks.
 */
void App_voidTriggerBuzzer(uint8_t Copy_u8BeepDurationTicks);

/**
 * @brief  Queues an event to be written into the external EEPROM log ring.
 */
void App_voidLogEvent(uint8_t Copy_u8EventId, uint8_t Copy_u8DeviceId, uint8_t Copy_u8NewState);

/* =========================================================================
 *                    COOPERATIVE RTOS TASK ENTRY POINTS
 * ========================================================================= */

/**
 * @brief  Task 0 (Period: 50 ms): Bluetooth command reception & parser.
 */
void App_Task_Bluetooth(void);

/**
 * @brief  Task 1 (Period: 50 ms): Manual inputs debouncing & override logic.
 */
void App_Task_ManualInputs(void);

/**
 * @brief  Task 2 (Period: 100 ms): Physical actuator update (LEDs, Relay, PWM Fan).
 */
void App_Task_ActuatorControl(void);

/**
 * @brief  Task 3 (Period: 250 ms): Character LCD 16x2 status update.
 */
void App_Task_LCDDisplay(void);

/**
 * @brief  Task 4 (Period: 500 ms): System heartbeat LED, uptime counter, buzzer timer.
 */
void App_Task_SystemMonitoring(void);

/**
 * @brief  Task 5 (Period: 1000 ms): Non-blocking EEPROM persistent log writer.
 */
void App_Task_EEPROMManager(void);

#endif /* APP_APP_H_ */
