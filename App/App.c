/*
 * App.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Project One: Bluetooth-Controlled Home Automation System
 *  Complete layered application logic, command parser, manual override,
 *  LCD status renderer, EEPROM usage logger, and cooperative RTOS tasks.
 */

#include <stdint.h>
#include <util/delay.h>

#include "../Common/BitMath.h"
#include "../Common/Definition.h"
#include "../Common/Config.h"

/* MCAL Layer */
#include "../Mcal/DIO/DIO_Interface.h"
#include "../Mcal/UART/UART_Interface.h"
#include "../Mcal/Timer1/T1_Interface.h"
#include "../Mcal/GIE/GIE_Interface.h"

/* HAL Layer */
#include "../Hal/LCD/LCD_Interface.h"
#include "../Hal/EEPROM/EEPROM_Interface.h"
#include "../Hal/DcMotor/DcMotor_Interface.h"
#include "../Hal/Relay/Relay_Interface.h"
#include "../Hal/Led/Led_Interface.h"
#include "../Hal/Button/Btn_Interface.h"
#include "../Hal/Buzzer/Buzzer_Interface.h"

/* App Layer */
#include "App_Config.h"
#include "App.h"

/* =========================================================================
 *                         INTERNAL CONFIGURATIONS
 * ========================================================================= */

/* DC Motor Configuration (L298/L293 H-Bridge simulation on PB5, PB6, PWM PD5) */
static Dc_config_t g_FanMotorConfig = {
    Dc_WithHBridge,
    APP_FAN_M1_PORT, APP_FAN_M1_PIN,
    APP_FAN_M2_PORT, APP_FAN_M2_PIN
};

/* Global system state */
static App_SystemState_t g_SystemState = {
    APP_STATE_OFF,               /* Light1 */
    APP_STATE_OFF,               /* Light2 */
    APP_STATE_OFF,               /* Relay */
    APP_FAN_SPEED_0,             /* Fan Speed */
    APP_MODE_BLUETOOTH,          /* Control Mode */
    0,                           /* Uptime */
    0                            /* Log Count */
};

/* UART RX Ring Buffer */
static volatile uint8_t g_u8UartRxQueue[APP_UART_RX_BUFFER_SIZE];
static volatile uint8_t g_u8UartHead = 0;
static volatile uint8_t g_u8UartTail = 0;

/* Auditory feedback counter (in 50ms task ticks) */
static volatile uint8_t g_u8BuzzerCountdown = 0;

/* External EEPROM pending log structure */
typedef struct
{
    App_LogRecord_t Record;
    uint8_t         Pending;
} PendingLog_t;

static PendingLog_t g_PendingLog = { {0, 0, 0, 0}, 0 };

/* =========================================================================
 *                      INTERNAL HELPER FUNCTIONS
 * ========================================================================= */

/* UART RX ISR callback */
static void App_voidUartRxCallback(uint8_t Copy_u8Byte)
{
    uint8_t Local_u8NextHead = (uint8_t)((g_u8UartHead + 1) % APP_UART_RX_BUFFER_SIZE);
    if(Local_u8NextHead != g_u8UartTail)
    {
        g_u8UartRxQueue[g_u8UartHead] = Copy_u8Byte;
        g_u8UartHead = Local_u8NextHead;
    }
}

/* Dequeue character from UART buffer */
static uint8_t App_u8UartPop(uint8_t *Copy_pu8Byte)
{
    if(g_u8UartHead == g_u8UartTail)
    {
        return 0; /* Empty */
    }
    *Copy_pu8Byte = g_u8UartRxQueue[g_u8UartTail];
    g_u8UartTail = (uint8_t)((g_u8UartTail + 1) % APP_UART_RX_BUFFER_SIZE);
    return 1;
}

/* Transmit 16-bit integer as ASCII over UART */
static void App_voidUartSendNumber(uint32_t Copy_u32Number)
{
    char Local_Buffer[11];
    int8_t Local_s8Idx = 0;

    if(Copy_u32Number == 0)
    {
        UART_voidSendData('0');
        return;
    }

    while(Copy_u32Number > 0)
    {
        Local_Buffer[Local_s8Idx++] = (char)((Copy_u32Number % 10) + '0');
        Copy_u32Number /= 10;
    }

    while(--Local_s8Idx >= 0)
    {
        UART_voidSendData((uint8_t)Local_Buffer[Local_s8Idx]);
    }
}

/* Write formatted system status to UART */
static void App_voidSendFullStatusOverBluetooth(void)
{
    UART_voidSendString("\r\n=== HOME AUTOMATION STATUS ===\r\n");
    UART_voidSendString("Mode:    ");
    UART_voidSendString((g_SystemState.ControlMode == APP_MODE_MANUAL_OVERRIDE) ? "MANUAL OVERRIDE\r\n" : "BLUETOOTH\r\n");

    UART_voidSendString("Light 1: ");
    UART_voidSendString((g_SystemState.Light1_State == APP_STATE_ON) ? "ON\r\n" : "OFF\r\n");

    UART_voidSendString("Light 2: ");
    UART_voidSendString((g_SystemState.Light2_State == APP_STATE_ON) ? "ON\r\n" : "OFF\r\n");

    UART_voidSendString("Relay:   ");
    UART_voidSendString((g_SystemState.Relay_State == APP_STATE_ON) ? "ON\r\n" : "OFF\r\n");

    UART_voidSendString("Fan:     ");
    App_voidUartSendNumber(g_SystemState.Fan_Speed);
    UART_voidSendString("%\r\n");

    UART_voidSendString("Uptime:  ");
    App_voidUartSendNumber(g_SystemState.UptimeSeconds);
    UART_voidSendString(" sec\r\n");

    UART_voidSendString("Logs:    ");
    App_voidUartSendNumber(g_SystemState.LogCount);
    UART_voidSendString(" records\r\n");
    UART_voidSendString("==============================\r\n");
}

/* Dump external EEPROM records over UART */
static void App_voidDumpEEPROMLogs(void)
{
    uint8_t Local_u8Total = g_SystemState.LogCount;
    uint8_t Local_u8Idx;

    UART_voidSendString("\r\n--- EEPROM USAGE LOGS ---\r\n");
    if(Local_u8Total == 0)
    {
        UART_voidSendString("No logs recorded yet.\r\n");
        return;
    }

    for(Local_u8Idx = 0; Local_u8Idx < Local_u8Total && Local_u8Idx < APP_EEPROM_MAX_ENTRIES; Local_u8Idx++)
    {
        uint16_t Local_u16Addr = APP_EEPROM_BASE_ADDR + (Local_u8Idx * APP_EEPROM_ENTRY_SIZE);
        uint8_t  Local_u8Event  = EEPROM_u8ReadByte(Local_u16Addr);
        uint8_t  Local_u8Dev    = EEPROM_u8ReadByte(Local_u16Addr + 1);
        uint8_t  Local_u8State  = EEPROM_u8ReadByte(Local_u16Addr + 2);
        uint8_t  Local_u8TimeH  = EEPROM_u8ReadByte(Local_u16Addr + 3);
        uint8_t  Local_u8TimeL  = EEPROM_u8ReadByte(Local_u16Addr + 4);
        uint16_t Local_u16Time  = ((uint16_t)Local_u8TimeH << 8) | Local_u8TimeL;

        UART_voidSendString("#");
        App_voidUartSendNumber(Local_u8Idx + 1);
        UART_voidSendString(" T:");
        App_voidUartSendNumber(Local_u16Time);
        UART_voidSendString("s | Dev:");
        switch(Local_u8Dev)
        {
            case APP_DEVICE_LIGHT1: UART_voidSendString("Light1"); break;
            case APP_DEVICE_LIGHT2: UART_voidSendString("Light2"); break;
            case APP_DEVICE_RELAY:  UART_voidSendString("Relay");  break;
            case APP_DEVICE_FAN:    UART_voidSendString("Fan");    break;
            case APP_DEVICE_ALL:    UART_voidSendString("All");    break;
            default:                UART_voidSendString("Unknown"); break;
        }

        UART_voidSendString(" | State:");
        App_voidUartSendNumber(Local_u8State);

        UART_voidSendString(" | Src:");
        switch(Local_u8Event)
        {
            case APP_EVENT_BLUETOOTH_CMD: UART_voidSendString("Bluetooth\r\n"); break;
            case APP_EVENT_MANUAL_BUTTON: UART_voidSendString("Button\r\n");    break;
            case APP_EVENT_MANUAL_DIP:    UART_voidSendString("DIP Switch\r\n"); break;
            case APP_EVENT_MASTER_SWITCH: UART_voidSendString("Master\r\n");    break;
            default:                      UART_voidSendString("Other\r\n");     break;
        }
    }
    UART_voidSendString("-------------------------\r\n");
}

/* Clear EEPROM logs */
static void App_voidClearEEPROMLogs(void)
{
    g_SystemState.LogCount = 0;
    EEPROM_voidWriteByte(APP_EEPROM_COUNT_ADDR, 0);
    UART_voidSendString("[ACK] EEPROM logs cleared.\r\n");
}

/* =========================================================================
 *                         PUBLIC API IMPLEMENTATION
 * ========================================================================= */

void App_voidInit(void)
{
    /* 1. Initialize System & Room Indicator LEDs */
    Led_Init(APP_LED_LIGHT1_PORT, APP_LED_LIGHT1_PIN);
    Led_Init(APP_LED_LIGHT2_PORT, APP_LED_LIGHT2_PIN);
    Led_Init(APP_LED_STATUS_PORT, APP_LED_STATUS_PIN);

    /* 2. Initialize Buzzer for auditory feedback */
    Buzzer_Init();

    /* 3. Initialize Relay module */
    Relay_Init();

    /* 4. Initialize DC Motor & Timer1 Fast PWM on OC1A (PD5) */
    DC_Init(&g_FanMotorConfig);
    MT1_voidFastPWMInit();
    DC_SetSpeed(0);
    DC_Off(&g_FanMotorConfig);

    /* 5. Initialize Manual Push Buttons (Internal Pull-Up) */
    BTN_Init(APP_BTN_LIGHT1_PORT,    APP_BTN_LIGHT1_PIN,    Btn_InternalPullup);
    BTN_Init(APP_BTN_FAN_CYCLE_PORT, APP_BTN_FAN_CYCLE_PIN, Btn_InternalPullup);
    BTN_Init(APP_BTN_RELAY_PORT,     APP_BTN_RELAY_PIN,     Btn_InternalPullup);

    /* 6. Initialize DIP Switches for Manual Override (Internal Pull-Up) */
    BTN_Init(APP_DIP_OVERRIDE_EN_PORT,     APP_DIP_OVERRIDE_EN_PIN,     Btn_InternalPullup);
    BTN_Init(APP_DIP_LIGHT2_OVERRIDE_PORT, APP_DIP_LIGHT2_OVERRIDE_PIN, Btn_InternalPullup);
    BTN_Init(APP_DIP_RELAY_OVERRIDE_PORT,  APP_DIP_RELAY_OVERRIDE_PIN,  Btn_InternalPullup);

    /* 7. Initialize 4-bit Character LCD */
    LCD_Init();
    LCD_SendCommand(Lcd_ClearScreen);
    _delay_ms(2);
    LCD_MoveTo(Lcd_Line1, 0);
    LCD_WriteString((uint8_t*)"  HOME AUTOMATION ");
    LCD_MoveTo(Lcd_Line2, 0);
    LCD_WriteString((uint8_t*)"  BLUETOOTH READY ");
    _delay_ms(1000);

    /* 8. Initialize External EEPROM via TWI */
    EEPROM_voidInit();
    /* Check magic key */
    if(EEPROM_u8ReadByte(APP_EEPROM_MAGIC_ADDR) != APP_EEPROM_MAGIC_VAL)
    {
        EEPROM_voidWriteByte(APP_EEPROM_MAGIC_ADDR, APP_EEPROM_MAGIC_VAL);
        EEPROM_voidWriteByte(APP_EEPROM_COUNT_ADDR, 0);
        g_SystemState.LogCount = 0;
    }
    else
    {
        g_SystemState.LogCount = EEPROM_u8ReadByte(APP_EEPROM_COUNT_ADDR);
        if(g_SystemState.LogCount > APP_EEPROM_MAX_ENTRIES)
        {
            g_SystemState.LogCount = APP_EEPROM_MAX_ENTRIES;
        }
    }

    /* 9. Initialize UART & register RX Interrupt callback */
    UART_voidInit();
    UART_voidSetRXCCallBack(&App_voidUartRxCallback);
    UART_voidEnableRXCInterrupt();

    /* Short welcome beep */
    App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);

    /* Ready banner over Bluetooth UART */
    UART_voidSendString("\r\n=========================================\r\n");
    UART_voidSendString("  Bluetooth Home Automation Online\r\n");
    UART_voidSendString("  Send 'S' for status, 'L' for log records\r\n");
    UART_voidSendString("=========================================\r\n");
}

const App_SystemState_t* App_psysGetState(void)
{
    return &g_SystemState;
}

void App_voidTriggerBuzzer(uint8_t Copy_u8BeepDurationTicks)
{
    Buzzer_On();
    g_u8BuzzerCountdown = Copy_u8BeepDurationTicks;
}

void App_voidLogEvent(uint8_t Copy_u8EventId, uint8_t Copy_u8DeviceId, uint8_t Copy_u8NewState)
{
    g_PendingLog.Record.EventId      = Copy_u8EventId;
    g_PendingLog.Record.DeviceId     = Copy_u8DeviceId;
    g_PendingLog.Record.NewState     = Copy_u8NewState;
    g_PendingLog.Record.TimestampSec = (uint16_t)g_SystemState.UptimeSeconds;
    g_PendingLog.Pending             = 1;
}

/* =========================================================================
 *                         RTOS TASK IMPLEMENTATIONS
 * ========================================================================= */

/**
 * Task 0: Bluetooth Command Reception & Parser
 * Runs every 50 ms.
 */
void App_Task_Bluetooth(void)
{
    uint8_t Local_u8Cmd;

    while(App_u8UartPop(&Local_u8Cmd))
    {
        /* Ignore whitespace and CRLF */
        if(Local_u8Cmd == '\r' || Local_u8Cmd == '\n' || Local_u8Cmd == ' ')
        {
            continue;
        }

        /* Check if system is locked in manual override mode */
        if(g_SystemState.ControlMode == APP_MODE_MANUAL_OVERRIDE &&
           Local_u8Cmd != APP_CMD_STATUS_REQ &&
           Local_u8Cmd != APP_CMD_LOG_DUMP &&
           Local_u8Cmd != APP_CMD_LOG_CLEAR)
        {
            UART_voidSendString("[WARN] Manual Override active. Switch DIP1 to unlock.\r\n");
            App_voidTriggerBuzzer(1);
            continue;
        }

        switch(Local_u8Cmd)
        {
            case APP_CMD_LIGHT1_ON:
                g_SystemState.Light1_State = APP_STATE_ON;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_LIGHT1, APP_STATE_ON);
                UART_voidSendString("[ACK] Light 1: ON\r\n");
                break;

            case APP_CMD_LIGHT1_OFF:
                g_SystemState.Light1_State = APP_STATE_OFF;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_LIGHT1, APP_STATE_OFF);
                UART_voidSendString("[ACK] Light 1: OFF\r\n");
                break;

            case APP_CMD_LIGHT2_ON:
                g_SystemState.Light2_State = APP_STATE_ON;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_LIGHT2, APP_STATE_ON);
                UART_voidSendString("[ACK] Light 2: ON\r\n");
                break;

            case APP_CMD_LIGHT2_OFF:
                g_SystemState.Light2_State = APP_STATE_OFF;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_LIGHT2, APP_STATE_OFF);
                UART_voidSendString("[ACK] Light 2: OFF\r\n");
                break;

            case APP_CMD_RELAY_ON:
                g_SystemState.Relay_State = APP_STATE_ON;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_RELAY, APP_STATE_ON);
                UART_voidSendString("[ACK] Relay: ON\r\n");
                break;

            case APP_CMD_RELAY_OFF:
                g_SystemState.Relay_State = APP_STATE_OFF;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_RELAY, APP_STATE_OFF);
                UART_voidSendString("[ACK] Relay: OFF\r\n");
                break;

            case APP_CMD_FAN_OFF:
                g_SystemState.Fan_Speed = APP_FAN_SPEED_0;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_FAN, APP_FAN_SPEED_0);
                UART_voidSendString("[ACK] Fan: OFF\r\n");
                break;

            case APP_CMD_FAN_LOW:
                g_SystemState.Fan_Speed = APP_FAN_SPEED_1_LOW;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_FAN, APP_FAN_SPEED_1_LOW);
                UART_voidSendString("[ACK] Fan: LOW (35%)\r\n");
                break;

            case APP_CMD_FAN_MED:
                g_SystemState.Fan_Speed = APP_FAN_SPEED_2_MED;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_FAN, APP_FAN_SPEED_2_MED);
                UART_voidSendString("[ACK] Fan: MED (70%)\r\n");
                break;

            case APP_CMD_FAN_HIGH:
                g_SystemState.Fan_Speed = APP_FAN_SPEED_3_HIGH;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_BLUETOOTH_CMD, APP_DEVICE_FAN, APP_FAN_SPEED_3_HIGH);
                UART_voidSendString("[ACK] Fan: HIGH (100%)\r\n");
                break;

            case APP_CMD_MASTER_ON:
                g_SystemState.Light1_State = APP_STATE_ON;
                g_SystemState.Light2_State = APP_STATE_ON;
                g_SystemState.Relay_State  = APP_STATE_ON;
                g_SystemState.Fan_Speed    = APP_FAN_SPEED_3_HIGH;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_MASTER_SWITCH, APP_DEVICE_ALL, APP_STATE_ON);
                UART_voidSendString("[ACK] MASTER ON: All devices active\r\n");
                break;

            case APP_CMD_MASTER_OFF:
                g_SystemState.Light1_State = APP_STATE_OFF;
                g_SystemState.Light2_State = APP_STATE_OFF;
                g_SystemState.Relay_State  = APP_STATE_OFF;
                g_SystemState.Fan_Speed    = APP_FAN_SPEED_0;
                App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
                App_voidLogEvent(APP_EVENT_MASTER_SWITCH, APP_DEVICE_ALL, APP_STATE_OFF);
                UART_voidSendString("[ACK] MASTER OFF: All devices idle\r\n");
                break;

            case APP_CMD_STATUS_REQ:
                App_voidSendFullStatusOverBluetooth();
                break;

            case APP_CMD_LOG_DUMP:
                App_voidDumpEEPROMLogs();
                break;

            case APP_CMD_LOG_CLEAR:
                App_voidClearEEPROMLogs();
                break;

            default:
                UART_voidSendString("[ERR] Unknown command: '");
                UART_voidSendData(Local_u8Cmd);
                UART_voidSendString("'. Send 'S' for status.\r\n");
                break;
        }
    }
}

/**
 * Task 1: Manual Input Handling & Debounce
 * Runs every 50 ms.
 */
void App_Task_ManualInputs(void)
{
    static uint8_t Local_u8Btn1LastState = PullUp_NotPressed;
    static uint8_t Local_u8Btn2LastState = PullUp_NotPressed;
    static uint8_t Local_u8Btn3LastState = PullUp_NotPressed;

    uint8_t Local_u8DipOverrideEn;
    uint8_t Local_u8DipLight2;
    uint8_t Local_u8DipRelay;

    uint8_t Local_u8Btn1Current;
    uint8_t Local_u8Btn2Current;
    uint8_t Local_u8Btn3Current;

    /* 1. Read DIP Switches (Low = ON with pull-up) */
    Local_u8DipOverrideEn = BTN_GetState(APP_DIP_OVERRIDE_EN_PORT, APP_DIP_OVERRIDE_EN_PIN, Btn_InternalPullup);
    Local_u8DipLight2     = BTN_GetState(APP_DIP_LIGHT2_OVERRIDE_PORT, APP_DIP_LIGHT2_OVERRIDE_PIN, Btn_InternalPullup);
    Local_u8DipRelay      = BTN_GetState(APP_DIP_RELAY_OVERRIDE_PORT, APP_DIP_RELAY_OVERRIDE_PIN, Btn_InternalPullup);

    if(Local_u8DipOverrideEn == PullUp_Pressed)
    {
        /* Manual override actively engaged */
        if(g_SystemState.ControlMode != APP_MODE_MANUAL_OVERRIDE)
        {
            g_SystemState.ControlMode = APP_MODE_MANUAL_OVERRIDE;
            App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
            UART_voidSendString("[INFO] Manual Override ENGAGED\r\n");
            App_voidLogEvent(APP_EVENT_MANUAL_DIP, APP_DEVICE_ALL, APP_MODE_MANUAL_OVERRIDE);
        }

        /* Direct DIP control */
        g_SystemState.Light2_State = (Local_u8DipLight2 == PullUp_Pressed) ? APP_STATE_ON : APP_STATE_OFF;
        g_SystemState.Relay_State  = (Local_u8DipRelay  == PullUp_Pressed) ? APP_STATE_ON : APP_STATE_OFF;
    }
    else
    {
        if(g_SystemState.ControlMode != APP_MODE_BLUETOOTH)
        {
            g_SystemState.ControlMode = APP_MODE_BLUETOOTH;
            App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
            UART_voidSendString("[INFO] Bluetooth Control RESTORED\r\n");
            App_voidLogEvent(APP_EVENT_MANUAL_DIP, APP_DEVICE_ALL, APP_MODE_BLUETOOTH);
        }
    }

    /* 2. Read and Debounce Push Button 1 (Toggle Light 1) */
    Local_u8Btn1Current = BTN_GetState(APP_BTN_LIGHT1_PORT, APP_BTN_LIGHT1_PIN, Btn_InternalPullup);
    if(Local_u8Btn1Current == PullUp_Pressed && Local_u8Btn1LastState == PullUp_NotPressed)
    {
        g_SystemState.Light1_State = (g_SystemState.Light1_State == APP_STATE_ON) ? APP_STATE_OFF : APP_STATE_ON;
        App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
        App_voidLogEvent(APP_EVENT_MANUAL_BUTTON, APP_DEVICE_LIGHT1, g_SystemState.Light1_State);
        UART_voidSendString("[BTN1] Light 1 Toggled\r\n");
    }
    Local_u8Btn1LastState = Local_u8Btn1Current;

    /* 3. Read and Debounce Push Button 2 (Cycle Fan Speed) */
    Local_u8Btn2Current = BTN_GetState(APP_BTN_FAN_CYCLE_PORT, APP_BTN_FAN_CYCLE_PIN, Btn_InternalPullup);
    if(Local_u8Btn2Current == PullUp_Pressed && Local_u8Btn2LastState == PullUp_NotPressed)
    {
        switch(g_SystemState.Fan_Speed)
        {
            case APP_FAN_SPEED_0:      g_SystemState.Fan_Speed = APP_FAN_SPEED_1_LOW;  break;
            case APP_FAN_SPEED_1_LOW:  g_SystemState.Fan_Speed = APP_FAN_SPEED_2_MED;  break;
            case APP_FAN_SPEED_2_MED:  g_SystemState.Fan_Speed = APP_FAN_SPEED_3_HIGH; break;
            case APP_FAN_SPEED_3_HIGH:
            default:                   g_SystemState.Fan_Speed = APP_FAN_SPEED_0;      break;
        }
        App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
        App_voidLogEvent(APP_EVENT_MANUAL_BUTTON, APP_DEVICE_FAN, g_SystemState.Fan_Speed);
        UART_voidSendString("[BTN2] Fan Speed Cycled\r\n");
    }
    Local_u8Btn2LastState = Local_u8Btn2Current;

    /* 4. Read and Debounce Push Button 3 (Toggle Relay - only if not DIP-forced) */
    if(g_SystemState.ControlMode != APP_MODE_MANUAL_OVERRIDE)
    {
        Local_u8Btn3Current = BTN_GetState(APP_BTN_RELAY_PORT, APP_BTN_RELAY_PIN, Btn_InternalPullup);
        if(Local_u8Btn3Current == PullUp_Pressed && Local_u8Btn3LastState == PullUp_NotPressed)
        {
            g_SystemState.Relay_State = (g_SystemState.Relay_State == APP_STATE_ON) ? APP_STATE_OFF : APP_STATE_ON;
            App_voidTriggerBuzzer(APP_BUZZER_BEEP_TICKS);
            App_voidLogEvent(APP_EVENT_MANUAL_BUTTON, APP_DEVICE_RELAY, g_SystemState.Relay_State);
            UART_voidSendString("[BTN3] Relay Toggled\r\n");
        }
        Local_u8Btn3LastState = Local_u8Btn3Current;
    }
}

/**
 * Task 2: Physical Actuator Control
 * Runs every 100 ms.
 */
void App_Task_ActuatorControl(void)
{
    /* 1. Update Room Light 1 (LED 1) */
    if(g_SystemState.Light1_State == APP_STATE_ON)
    {
        Led_On(APP_LED_LIGHT1_PORT, APP_LED_LIGHT1_PIN, Led_SourceConnection);
    }
    else
    {
        Led_Off(APP_LED_LIGHT1_PORT, APP_LED_LIGHT1_PIN, Led_SourceConnection);
    }

    /* 2. Update Room Light 2 (LED 2) */
    if(g_SystemState.Light2_State == APP_STATE_ON)
    {
        Led_On(APP_LED_LIGHT2_PORT, APP_LED_LIGHT2_PIN, Led_SourceConnection);
    }
    else
    {
        Led_Off(APP_LED_LIGHT2_PORT, APP_LED_LIGHT2_PIN, Led_SourceConnection);
    }

    /* 3. Update Relay */
    if(g_SystemState.Relay_State == APP_STATE_ON)
    {
        Relay_On();
    }
    else
    {
        Relay_Off();
    }

    /* 4. Update DC Motor / Fan */
    if(g_SystemState.Fan_Speed > 0)
    {
        DC_OnCW(&g_FanMotorConfig);
        DC_SetSpeed(g_SystemState.Fan_Speed);
    }
    else
    {
        DC_Off(&g_FanMotorConfig);
        DC_SetSpeed(0);
    }
}

/**
 * Task 3: 16x2 Character LCD Display
 * Runs every 250 ms.
 */
void App_Task_LCDDisplay(void)
{
    /* Line 1: "L1:ON  L2:OFF M:BT" (16 chars fixed) */
    LCD_MoveTo(Lcd_Line1, 0);
    LCD_WriteString((uint8_t*)"L1:");
    LCD_WriteString((g_SystemState.Light1_State == APP_STATE_ON) ? (uint8_t*)"ON " : (uint8_t*)"OFF");

    LCD_WriteString((uint8_t*)" L2:");
    LCD_WriteString((g_SystemState.Light2_State == APP_STATE_ON) ? (uint8_t*)"ON " : (uint8_t*)"OFF");

    LCD_WriteString((uint8_t*)" M:");
    LCD_WriteString((g_SystemState.ControlMode == APP_MODE_MANUAL_OVERRIDE) ? (uint8_t*)"MN" : (uint8_t*)"BT");

    /* Line 2: "R:ON  F:100% T:X" */
    LCD_MoveTo(Lcd_Line2, 0);
    LCD_WriteString((uint8_t*)"R:");
    LCD_WriteString((g_SystemState.Relay_State == APP_STATE_ON) ? (uint8_t*)"ON " : (uint8_t*)"OFF");

    LCD_WriteString((uint8_t*)" F:");
    switch(g_SystemState.Fan_Speed)
    {
        case APP_FAN_SPEED_0:      LCD_WriteString((uint8_t*)"  0%"); break;
        case APP_FAN_SPEED_1_LOW:  LCD_WriteString((uint8_t*)" 35%"); break;
        case APP_FAN_SPEED_2_MED:  LCD_WriteString((uint8_t*)" 70%"); break;
        case APP_FAN_SPEED_3_HIGH: LCD_WriteString((uint8_t*)"100%"); break;
        default:                   LCD_WriteString((uint8_t*)"---%"); break;
    }

    LCD_WriteString((uint8_t*)" ");
    LCD_WriteNumber((int32_t)(g_SystemState.UptimeSeconds % 1000));
    LCD_WriteString((uint8_t*)"s  ");
}

/**
 * Task 4: System Monitoring, Heartbeat & Buzzer Timer
 * Runs every 500 ms.
 */
void App_Task_SystemMonitoring(void)
{
    static uint8_t Local_u8HalfSecToggle = 0;

    /* Heartbeat LED on PB3 */
    Led_Toggle(APP_LED_STATUS_PORT, APP_LED_STATUS_PIN);

    /* Decrement buzzer duration */
    if(g_u8BuzzerCountdown > 0)
    {
        g_u8BuzzerCountdown--;
        if(g_u8BuzzerCountdown == 0)
        {
            Buzzer_Off();
        }
    }

    /* Increment uptime once every second (every 2 calls of 500ms) */
    Local_u8HalfSecToggle ^= 1;
    if(Local_u8HalfSecToggle == 0)
    {
        g_SystemState.UptimeSeconds++;
    }
}

/**
 * Task 5: External EEPROM Persistent Data Logger
 * Runs every 1000 ms.
 */
void App_Task_EEPROMManager(void)
{
    if(g_PendingLog.Pending)
    {
        uint8_t Local_u8Idx = g_SystemState.LogCount;
        uint16_t Local_u16Addr;

        if(Local_u8Idx >= APP_EEPROM_MAX_ENTRIES)
        {
            Local_u8Idx = 0; /* Ring buffer wrap */
        }

        Local_u16Addr = APP_EEPROM_BASE_ADDR + (Local_u8Idx * APP_EEPROM_ENTRY_SIZE);

        EEPROM_voidWriteByte(Local_u16Addr,     g_PendingLog.Record.EventId);
        EEPROM_voidWriteByte(Local_u16Addr + 1, g_PendingLog.Record.DeviceId);
        EEPROM_voidWriteByte(Local_u16Addr + 2, g_PendingLog.Record.NewState);
        EEPROM_voidWriteByte(Local_u16Addr + 3, (uint8_t)(g_PendingLog.Record.TimestampSec >> 8));
        EEPROM_voidWriteByte(Local_u16Addr + 4, (uint8_t)(g_PendingLog.Record.TimestampSec & 0xFF));

        if(g_SystemState.LogCount < APP_EEPROM_MAX_ENTRIES)
        {
            g_SystemState.LogCount++;
        }
        EEPROM_voidWriteByte(APP_EEPROM_COUNT_ADDR, g_SystemState.LogCount);

        g_PendingLog.Pending = 0;
    }
}
