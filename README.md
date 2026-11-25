[bluetooth_controlled_home_automation_system_readme.md](https://github.com/user-attachments/files/35405348/bluetooth_controlled_home_automation_system_readme.md)
<div align="center">

# 🏠 Bluetooth-Controlled Home Automation System

**A Layered Embedded C & Cooperative RTOS Firmware for AVR ATmega32**

[![MCU](https://img.shields.io/badge/MCU-ATmega32-blue?style=for-the-badge&logo=microchip)](https://www.microchip.com/)
[![Language](https://img.shields.io/badge/Language-Embedded%20C-00599C?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Clock](https://img.shields.io/badge/F__CPU-8%20MHz-orange?style=for-the-badge)](#)
[![Architecture](https://img.shields.io/badge/Architecture-Layered%20%2B%20RTOS-success?style=for-the-badge)](#system-architecture)
[![Protocol](https://img.shields.io/badge/Bluetooth-UART%209600%20bps-0082FC?style=for-the-badge&logo=bluetooth)](#bluetooth-command-protocol)

*Real-time wireless and manual smart-home appliance control featuring PWM fan speed regulation, hardware override locks, a live 16×2 LCD dashboard, and persistent I²C EEPROM event logging.*

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [System Architecture](#-system-architecture)
- [Cooperative RTOS Task Schedule](#-cooperative-rtos-task-schedule)
- [Hardware Pinout & Wiring](#-hardware-pinout--wiring)
- [Bluetooth Command Protocol](#-bluetooth-command-protocol)
- [External EEPROM Memory Map](#-external-eeprom-memory-map)
- [Repository Structure](#-repository-structure)
- [Build & Flash Instructions](#-build--flash-instructions)
- [Author](#-author)

---

## 🔍 Overview

This project implements a full-featured **Smart Home Automation Controller** on the **Microchip AVR ATmega32** microcontroller running at **8 MHz**. Designed using strict **Embedded Layered Architecture** principles (`Common` $\rightarrow$ `MCAL` $\rightarrow$ `HAL` $\rightarrow$ `RTOS` $\rightarrow$ `App`), the firmware decouples low-level register manipulation from hardware drivers and deterministic real-time task scheduling.

Users can control room lighting, a high-voltage relay load, and a variable-speed DC fan wirelessly via a **Bluetooth serial module (HC-05/HC-06)** or locally via **debounced push buttons** and a **3-channel DIP switch hardware override**.

---

## ✨ Key Features

- **Interrupt-Driven Bluetooth Control (UART @ 9600 bps, 8N1):** Uses the `USART_RXC` ISR with a 16-byte lock-free circular ring buffer so incoming mobile app commands are captured without blocking real-time tasks.
- **Custom Cooperative RTOS Scheduler:** Built on **Timer0 Normal Mode** (`10 ms` base tick), executing 6 prioritized tasks with staggered `FirstDelay` offsets to prevent CPU load spikes.
- **4-Level PWM Fan Speed Control:** Drives a DC motor via an H-Bridge (`L293D`/`L298N`) using **Timer1 Fast PWM Mode 14** (`ICR1 = 999`, `1 kHz` carrier on `OC1A/PD5`) across `0%`, `35%`, `70%`, and `100%` duty cycles.
- **Hardware Manual Override Mode:** Engaging `DIP1` (`PB0`) locks out remote Bluetooth appliance switching and hands direct control of Room Light 2 (`PB1`) and the Relay (`PB2`) to physical DIP switches while still permitting status and log diagnostics.
- **Live 16×2 Character LCD Dashboard (4-Bit Mode):** Displays real-time appliance states, active control mode (`BT` vs. `MN`), fan duty percentage, and live system uptime while keeping `PC0`/`PC1` free for I²C communication.
- **Persistent I²C (TWI) EEPROM Event Logger:** Non-blocking background task records up to **40 timestamped state-change events** into an external `24Cxx` EEPROM ring buffer, retrievable or clearable over Bluetooth.
- **Non-Blocking Auditory & Visual Feedback:** Provides 100 ms buzzer acknowledgments (`PA3`) on valid actions and a `500 ms` heartbeat status LED (`PB3`).

---

## 🏗 System Architecture

```text
+-----------------------------------------------------------------------+
|                       APPLICATION LAYER (App/)                        |
|  Bluetooth Parser | Manual Override | LCD UI | EEPROM Logger | State  |
+-----------------------------------+-----------------------------------+
                                    |
+-----------------------------------v-----------------------------------+
|                   SERVICE LAYER (RTOS/ - 10 ms Tick)                  |
|         Priority-Based Cooperative Scheduler (6 Periodic Tasks)       |
+-----------------------------------+-----------------------------------+
                                    |
+-----------------------------------v-----------------------------------+
|               HARDWARE ABSTRACTION LAYER (Hal/)                       |
|  LCD (4-bit) | EEPROM (24Cxx) | DcMotor | Relay | Led | Button | Buzz |
+-----------------------------------+-----------------------------------+
                                    |
+-----------------------------------v-----------------------------------+
|           MICROCONTROLLER ABSTRACTION LAYER (Mcal/)                   |
|   DIO | UART (RXC ISR) | Timer0 | Timer1 (Fast PWM) | TWI (I2C) | GIE |
+-----------------------------------+-----------------------------------+
                                    |
+-----------------------------------v-----------------------------------+
|                    COMMON UTILITIES (Common/)                         |
|           BitMath.h | Definition.h | Config.h (F_CPU = 8 MHz)         |
+-----------------------------------------------------------------------+
```

---

## ⏱ Cooperative RTOS Task Schedule

Timer0 is configured in **Normal Mode** (`Prescaler = 64`, `Preload = 6`, `5 overflows` per tick) to generate an exact **10 ms** system tick at $F_{\text{CPU}} = 8\text{ MHz}$. Each task uses a staggered `FirstDelay` so multiple tasks do not collide on the same tick:

| Priority | Task Entry Point | Period (Ticks) | Period (ms) | First Delay | Responsibility |
| :---: | :--- | :---: | :---: | :---: | :--- |
| **0** | `App_Task_Bluetooth` | 5 | `50 ms` | 0 ticks | Pops bytes from UART RX ring buffer, parses commands, sends ACKs |
| **1** | `App_Task_ManualInputs` | 5 | `50 ms` | 1 tick | Reads 3-channel DIP override switches & debounces 3 push buttons |
| **2** | `App_Task_ActuatorControl` | 10 | `100 ms` | 2 ticks | Applies target states to Light 1, Light 2, Relay, and Timer1 PWM Fan |
| **3** | `App_Task_LCDDisplay` | 25 | `250 ms` | 5 ticks | Refreshes 16×2 LCD status lines (`L1`, `L2`, `Mode`, `Relay`, `Fan`, `Uptime`) |
| **4** | `App_Task_SystemMonitoring` | 50 | `500 ms` | 10 ticks | Toggles heartbeat LED (`PB3`), updates 1s uptime counter, times buzzer |
| **5** | `App_Task_EEPROMManager` | 100 | `1000 ms` | 20 ticks | Writes queued appliance state-change events to external EEPROM via TWI |

---

## 🔌 Hardware Pinout & Wiring

| ATmega32 Pin | Direction | Peripheral / Module | Function & Configuration |
| :---: | :---: | :--- | :--- |
| **PA0** | Output | 16×2 LCD (`RS`) | Register Select control line |
| **PA1** | Output | 16×2 LCD (`RW`) | Read/Write control line (driven `LOW`) |
| **PA2** | Output | 16×2 LCD (`E`) | Enable strobe pulse |
| **PA3** | Output | Active Buzzer | NPN transistor driver (100 ms command feedback) |
| **PA4** | Output | Room Light 1 LED | Source-connected indicator/load LED |
| **PA5** | Output | Room Light 2 LED | Source-connected indicator/load LED |
| **PA6** | Input | Push Button 1 | Manual toggle for **Light 1** (Internal Pull-Up) |
| **PA7** | Input | Push Button 2 | Manual speed cycle for **DC Fan** (Internal Pull-Up) |
| **PB0** | Input | DIP Switch 1 | **Master Manual Override Enable** (`LOW` = Override Active) |
| **PB1** | Input | DIP Switch 2 | Direct manual override for **Light 2** (Internal Pull-Up) |
| **PB2** | Input | DIP Switch 3 | Direct manual override for **Relay** (Internal Pull-Up) |
| **PB3** | Output | Status LED | System heartbeat indicator (`500 ms` toggle) |
| **PB4** | Output | Relay Module | High-voltage appliance control (Active-High) |
| **PB5** | Output | DC Motor (`IN1`) | H-Bridge direction control pin 1 |
| **PB6** | Output | DC Motor (`IN2`) | H-Bridge direction control pin 2 |
| **PC0** | Bidirectional | External EEPROM (`SCL`) | TWI / I²C serial clock line |
| **PC1** | Bidirectional | External EEPROM (`SDA`) | TWI / I²C serial data line |
| **PC2** | Input | Push Button 3 | Manual toggle for **Relay** (Internal Pull-Up) |
| **PC4 – PC7** | Output | 16×2 LCD (`D4–D7`) | 4-bit parallel data bus upper nibble |
| **PD0** | Input | Bluetooth (`RXD`) | Connects to HC-05/HC-06 `TXD` (`9600 bps`) |
| **PD1** | Output | Bluetooth (`TXD`) | Connects to HC-05/HC-06 `RXD` (`9600 bps`) |
| **PD5** | Output | DC Motor (`OC1A` / `EN`) | Timer1 Fast PWM (`1 kHz`, non-inverting) |

### LCD Display Format (16×2)

```text
+----------------+
|L1:ON  L2:OFF M:BT|  <- Line 1: Light 1, Light 2, Mode (BT = Bluetooth, MN = Manual)
|R:ON  F: 70% 124s |  <- Line 2: Relay state, Fan PWM duty %, Uptime in seconds
+----------------+
```

---

## 📡 Bluetooth Command Protocol

Connect any Bluetooth Serial Terminal or custom Android companion app at **9600 baud, 8 data bits, No parity, 1 stop bit (8N1)** and send single-character ASCII commands:

| Command | Action | Response / Behavior |
| :---: | :--- | :--- |
| `A` / `a` | Room Light 1 **ON** / **OFF** | `[ACK] Light 1: ON` / `OFF` + Buzzer beep + EEPROM log |
| `B` / `b` | Room Light 2 **ON** / **OFF** | `[ACK] Light 2: ON` / `OFF` + Buzzer beep + EEPROM log |
| `R` / `r` | Appliance Relay **ON** / **OFF** | `[ACK] Relay: ON` / `OFF` + Buzzer beep + EEPROM log |
| `F` | DC Fan **OFF** (`0%` PWM) | `[ACK] Fan: OFF` + Buzzer beep + EEPROM log |
| `1` | DC Fan **LOW** (`35%` PWM) | `[ACK] Fan: LOW (35%)` + Buzzer beep + EEPROM log |
| `2` | DC Fan **MEDIUM** (`70%` PWM) | `[ACK] Fan: MED (70%)` + Buzzer beep + EEPROM log |
| `3` | DC Fan **HIGH** (`100%` PWM) | `[ACK] Fan: HIGH (100%)` + Buzzer beep + EEPROM log |
| `#` | **Master ON** | Turns ON Light 1, Light 2, Relay, and sets Fan to `100%` |
| `*` | **Master OFF** | Turns OFF all lights, relay, and fan (`0%`) |
| `S` | **Status Report** | Streams full formatted report (Mode, Lights, Relay, Fan %, Uptime, Log count) |
| `L` | **Dump EEPROM Logs** | Streams all stored timestamped event records from external EEPROM |
| `C` | **Clear EEPROM Logs** | Resets the EEPROM event record counter to `0` |

> **Note:** When Hardware Manual Override (`DIP1` on `PB0`) is active, appliance switching commands return `[WARN] Manual Override active. Switch DIP1 to unlock.`, while diagnostic commands (`S`, `L`, `C`) remain accessible.

---

## 💾 External EEPROM Memory Map

The firmware interfaces with an external `24Cxx` I²C EEPROM (`0x50` base address) to persist appliance state transitions across power cycles using a **40-entry circular ring buffer**:

| Address Range | Size | Field | Description |
| :--- | :---: | :--- | :--- |
| `0x0000` | 1 Byte | `MAGIC_KEY` | Initialization signature (`0x5A`); formats log counter on first boot |
| `0x0001` | 1 Byte | `LOG_COUNT` | Number of valid records currently stored (`0` – `40`) |
| `0x0010` – `0x00D7` | 200 Bytes | `LOG_RING[40]` | 40 entries × 5 bytes each: `[EventId, DeviceId, NewState, Time_H, Time_L]` |

---

## 📁 Repository Structure

```text
Bluetooth-Controlled-Home-Automation-System/
├── main.c                          # System initialization & RTOS task registration
├── Common/
│   ├── BitMath.h                   # Bitwise register manipulation macros
│   ├── Config.h                    # F_CPU (8 MHz) & module enable switches
│   └── Definition.h                # Standard macros & null definitions
├── Mcal/                           # Microcontroller Abstraction Layer (ATmega32)
│   ├── Atmega32Registers.h         # Memory-mapped hardware register definitions
│   ├── ADC/                        # Analog-to-Digital Converter driver
│   ├── DIO/                        # Digital I/O port/pin driver
│   ├── EXTI/                       # External Interrupts (INT0, INT1, INT2)
│   ├── GIE/                        # Global Interrupt Enable (SREG I-bit)
│   ├── SPI/                        # Serial Peripheral Interface driver
│   ├── Timer0/                     # 8-bit Timer0 (Normal OVF for RTOS 10 ms tick & PWM)
│   ├── Timer1/                     # 16-bit Timer1 (Mode 14 Fast PWM on OC1A)
│   ├── TWI/                        # Two-Wire Interface (I2C Master/Slave)
│   └── UART/                       # USART driver with RX Complete interrupt callback
├── Hal/                            # Hardware Abstraction Layer
│   ├── Button/                     # Tactile push buttons & DIP switch reader
│   ├── Buzzer/                     # Active buzzer driver (NPN/PNP configurable)
│   ├── DcMotor/                    # DC motor H-Bridge & PWM speed wrapper
│   ├── EEPROM/                     # External 24Cxx I2C EEPROM driver
│   ├── KPD/                        # 4x4 Matrix Keypad driver
│   ├── LCD/                        # HD44780 16x2 Character LCD (4-bit & 8-bit modes)
│   ├── Led/                        # Source/Sink LED driver
│   ├── LM35/                       # LM35 analog temperature sensor driver
│   ├── Relay/                      # Electromagnetic relay driver
│   ├── SevSeg/                     # 7-Segment display driver
│   └── TempSensor/                 # Generic ADC temperature sensor driver
├── RTOS/                           # Cooperative Real-Time Operating System
│   ├── RTOS_Config.h               # Max concurrent tasks (RTOS_MAX_NUM_TASKS = 6)
│   ├── RTOS_Interface.h            # Create, Suspend, Resume, Delete task APIs
│   ├── RTOS_Private.h              # TCB struct & task states
│   └── RTOS_Program.c              # Priority-based scheduler kernel
└── App/                            # Application Layer
    ├── App_Config.h                # Pin mappings, UART commands & EEPROM offsets
    ├── App.h                       # System state structures & RTOS task prototypes
    └── App.c                       # Core state machine, parser, UI & EEPROM logger
```

---

## 🛠 Build & Flash Instructions

### Option 1: Eclipse IDE for C/C++ Developers (AVR-Eclipse Plugin)
1. Clone the repository:
   ```bash
   git clone https://github.com/Mohamed-Tapl/Bluetooth-Controlled-Home-Automation-System.git
   ```
2. Open **Eclipse** and select **File $\rightarrow$ Import $\rightarrow$ General $\rightarrow$ Existing Projects into Workspace**.
3. Browse to the cloned directory and click **Finish**.
4. Verify target settings under **Project $\rightarrow$ Properties $\rightarrow$ AVR $\rightarrow$ Target Hardware**:
   - **MCU Type:** `ATmega32`
   - **MCU Clock Frequency:** `8000000 Hz`
5. Build the project (**Ctrl + B**) to generate the `.hex` binary inside `Debug/`.

### Option 2: Command Line (`avr-gcc` & `avrdude`)
```bash
# Compile all source files for ATmega32 @ 8 MHz
avr-gcc -mmcu=atmega32 -DF_CPU=8000000UL -Os -Wall \
  main.c App/*.c RTOS/*.c Mcal/*/*.c Hal/*/*.c \
  -o HomeAutomation.elf

# Generate Intel HEX file
avr-objcopy -O ihex -R .eeprom HomeAutomation.elf HomeAutomation.hex

# Flash to ATmega32 via USBasp programmer
avrdude -c usbasp -p m32 -U flash:w:HomeAutomation.hex:i
```

> **Fuse Bits Reminder:** Ensure the ATmega32 fuse bits are configured for the **8 MHz Internal RC Oscillator** (or external 8 MHz crystal) so UART baud rate generation (`9600 bps`) and RTOS timing (`10 ms` tick) remain accurate.

---

## 👤 Author

**Mohamed Tapl**
- GitHub: [@Mohamed-Tapl](https://github.com/Mohamed-Tapl)
