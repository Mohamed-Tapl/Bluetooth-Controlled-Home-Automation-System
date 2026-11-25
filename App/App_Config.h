/*
 * App_Config.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Mohamed Tapl
 *
 *  Project One: Bluetooth-Controlled Home Automation System
 *  Central pin configuration, command characters, and EEPROM layout.
 */

#ifndef APP_APP_CONFIG_H_
#define APP_APP_CONFIG_H_

#include <stdint.h>
#include "../Common/Config.h"
#include "../Common/Definition.h"
#include "../Mcal/DIO/DIO_Interface.h"

/* =========================================================================
 *                         HARDWARE PIN MAPPINGS
 * ========================================================================= */

/* --- Status & Room Indicator LEDs --- */
#define APP_LED_LIGHT1_PORT             DIO_GroupA
#define APP_LED_LIGHT1_PIN              DIO_Pin4

#define APP_LED_LIGHT2_PORT             DIO_GroupA
#define APP_LED_LIGHT2_PIN              DIO_Pin5

#define APP_LED_STATUS_PORT             DIO_GroupB
#define APP_LED_STATUS_PIN              DIO_Pin3

/* --- Auditory Feedback (Buzzer) --- */
#define APP_BUZZER_PORT                 DIO_GroupA
#define APP_BUZZER_PIN                  DIO_Pin3

/* --- Manual Control Push Buttons (Internal Pull-Up) --- */
#define APP_BTN_LIGHT1_PORT             DIO_GroupA
#define APP_BTN_LIGHT1_PIN              DIO_Pin6

#define APP_BTN_FAN_CYCLE_PORT          DIO_GroupA
#define APP_BTN_FAN_CYCLE_PIN           DIO_Pin7

#define APP_BTN_RELAY_PORT              DIO_GroupC
#define APP_BTN_RELAY_PIN               DIO_Pin2

/* --- DIP Switches for Manual Override Mode --- */
#define APP_DIP_OVERRIDE_EN_PORT        DIO_GroupB
#define APP_DIP_OVERRIDE_EN_PIN         DIO_Pin0

#define APP_DIP_LIGHT2_OVERRIDE_PORT    DIO_GroupB
#define APP_DIP_LIGHT2_OVERRIDE_PIN     DIO_Pin1

#define APP_DIP_RELAY_OVERRIDE_PORT     DIO_GroupB
#define APP_DIP_RELAY_OVERRIDE_PIN      DIO_Pin2

/* --- Relay Module (PB4) --- */
#define APP_RELAY_PORT                  DIO_GroupB
#define APP_RELAY_PIN                   DIO_Pin4

/* --- DC Motor (Fan Simulation) --- */
#define APP_FAN_M1_PORT                 DIO_GroupB
#define APP_FAN_M1_PIN                  DIO_Pin5

#define APP_FAN_M2_PORT                 DIO_GroupB
#define APP_FAN_M2_PIN                  DIO_Pin6

#define APP_FAN_PWM_PORT                DIO_GroupD
#define APP_FAN_PWM_PIN                 DIO_Pin5   /* Timer1 OC1A (Fast PWM) */

/* --- Fan Speed Levels (PWM Duty Cycles in %) --- */
#define APP_FAN_SPEED_0                 0
#define APP_FAN_SPEED_1_LOW             35
#define APP_FAN_SPEED_2_MED             70
#define APP_FAN_SPEED_3_HIGH            100

/* =========================================================================
 *                    BLUETOOTH COMMAND PROTOCOL CHARS
 * ========================================================================= */
#define APP_CMD_LIGHT1_ON               'A'
#define APP_CMD_LIGHT1_OFF              'a'
#define APP_CMD_LIGHT2_ON               'B'
#define APP_CMD_LIGHT2_OFF              'b'

#define APP_CMD_RELAY_ON                'R'
#define APP_CMD_RELAY_OFF               'r'

#define APP_CMD_FAN_OFF                 'F'
#define APP_CMD_FAN_LOW                 '1'
#define APP_CMD_FAN_MED                 '2'
#define APP_CMD_FAN_HIGH                '3'

#define APP_CMD_MASTER_ON               '#'
#define APP_CMD_MASTER_OFF              '*'

#define APP_CMD_STATUS_REQ              'S'
#define APP_CMD_LOG_DUMP                'L'
#define APP_CMD_LOG_CLEAR               'C'

/* =========================================================================
 *                         EEPROM LOGGING LAYOUT
 * ========================================================================= */
#define APP_EEPROM_MAGIC_ADDR           0x0000
#define APP_EEPROM_MAGIC_VAL            0x5A

#define APP_EEPROM_COUNT_ADDR           0x0001
#define APP_EEPROM_BASE_ADDR            0x0010
#define APP_EEPROM_MAX_ENTRIES          40
#define APP_EEPROM_ENTRY_SIZE           5

/* Event IDs for Logging */
#define APP_EVENT_BLUETOOTH_CMD         0x01
#define APP_EVENT_MANUAL_BUTTON         0x02
#define APP_EVENT_MANUAL_DIP            0x03
#define APP_EVENT_MASTER_SWITCH         0x04

/* Appliance IDs for Logging */
#define APP_DEVICE_LIGHT1               0x01
#define APP_DEVICE_LIGHT2               0x02
#define APP_DEVICE_RELAY                0x03
#define APP_DEVICE_FAN                  0x04
#define APP_DEVICE_ALL                  0x0F

/* =========================================================================
 *                        SYSTEM TIMINGS & QUEUES
 * ========================================================================= */
#define APP_UART_RX_BUFFER_SIZE         16
#define APP_BUZZER_BEEP_TICKS           2   /* 2 x 50 ms task tick = 100 ms beep */

#endif /* APP_APP_CONFIG_H_ */
