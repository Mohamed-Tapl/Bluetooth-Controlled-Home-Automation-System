/*
 * UART_Program.c
 *
 *  Created on: Nov 10, 2026
 *      Author: Mohamed Tapl
 */
#include <stdint.h>
#include "../../Common/BitMath.h"
#include "../../Common/Definition.h"
#include "../Atmega32Registers.h"

#include "UART_Interface.h"
#include "UART_Private.h"
#include "UART_Config.h"

/* Private: RX callback pointer. Receives the byte that was just read. */
static void (*UART_pvRXCCallBack)(uint8_t) = NULL;

void UART_voidInit(void)
{
    uint8_t Local_u8UCSRCValue = 0;
    uint16_t Local_u16UBRRValue = (uint16_t)((UART_SYSTEM_FREQUENCY / (16UL * UART_BAUD_RATE)) - 1);


    SetBit(Local_u8UCSRCValue, UCSRC_URSEL);

#if UART_PARITY_MODE == UART_PARITY_DISABLE
    ClearBit(Local_u8UCSRCValue, UCSRC_UPM0);
    ClearBit(Local_u8UCSRCValue, UCSRC_UPM1);
#elif UART_PARITY_MODE == UART_PARITY_EVEN
    ClearBit(Local_u8UCSRCValue, UCSRC_UPM0);
    SetBit(Local_u8UCSRCValue, UCSRC_UPM1);
#elif UART_PARITY_MODE == UART_PARITY_ODD
    SetBit(Local_u8UCSRCValue, UCSRC_UPM0);
    SetBit(Local_u8UCSRCValue, UCSRC_UPM1);
#endif

#if UART_STOP_BITS == UART_STOP_1_BIT
    ClearBit(Local_u8UCSRCValue, UCSRC_USBS);
#elif UART_STOP_BITS == UART_STOP_2_BIT
    SetBit(Local_u8UCSRCValue, UCSRC_USBS);
#endif

#if UART_CHARACTER_SIZE == UART_DATA_5_BIT
    ClearBit(Local_u8UCSRCValue, UCSRC_UCSZ0);
    ClearBit(Local_u8UCSRCValue, UCSRC_UCSZ1);
    ClearBit(UCSRB_Reg, UCSRB_UCSZ2);
#elif UART_CHARACTER_SIZE == UART_DATA_6_BIT
    SetBit(Local_u8UCSRCValue, UCSRC_UCSZ0);
    ClearBit(Local_u8UCSRCValue, UCSRC_UCSZ1);
    ClearBit(UCSRB_Reg, UCSRB_UCSZ2);
#elif UART_CHARACTER_SIZE == UART_DATA_7_BIT
    ClearBit(Local_u8UCSRCValue, UCSRC_UCSZ0);
    SetBit(Local_u8UCSRCValue, UCSRC_UCSZ1);
    ClearBit(UCSRB_Reg, UCSRB_UCSZ2);
#elif UART_CHARACTER_SIZE == UART_DATA_8_BIT
    SetBit(Local_u8UCSRCValue, UCSRC_UCSZ0);
    SetBit(Local_u8UCSRCValue, UCSRC_UCSZ1);
    ClearBit(UCSRB_Reg, UCSRB_UCSZ2);
#endif


    UCSRC_Reg = Local_u8UCSRCValue;


    UBRRL_Reg = (uint8_t)Local_u16UBRRValue;
    UBRRH_Reg = (uint8_t)((Local_u16UBRRValue >> 8) & 0x7F);


    SetBit(UCSRB_Reg, UCSRB_RXEN);
    SetBit(UCSRB_Reg, UCSRB_TXEN);
}

void UART_voidSendData(uint8_t Copy_u8Data)
{

    while(ReadBit(UCSRA_Reg, UCSRA_UDRE) == 0);
    UDR_Reg = Copy_u8Data;
}

uint8_t UART_u8ReceiveData(void)
{

    while(ReadBit(UCSRA_Reg, UCSRA_RXC) == 0);
    return UDR_Reg;
}

void UART_voidSendString(const char* Copy_pcString)
{
    uint32_t Local_u32Index = 0;
    while(Copy_pcString[Local_u32Index] != '\0')
    {
        UART_voidSendData(Copy_pcString[Local_u32Index]);
        Local_u32Index++;
    }
}

/*------- RX Interrupt API -------*/

void UART_voidEnableRXCInterrupt(void)
{
    SetBit(UCSRB_Reg, UCSRB_RXCIE);
}

void UART_voidDisableRXCInterrupt(void)
{
    ClearBit(UCSRB_Reg, UCSRB_RXCIE);
}

void UART_voidSetRXCCallBack(void (*Copy_pvCallBack)(uint8_t))
{
    if(Copy_pvCallBack != NULL)
    {
        UART_pvRXCCallBack = Copy_pvCallBack;
    }
}

/* USART RX Complete ISR - Vector 13 on ATmega32 */
void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
    uint8_t Local_u8ReceivedByte = UDR_Reg;
    if(UART_pvRXCCallBack != NULL)
    {
        UART_pvRXCCallBack(Local_u8ReceivedByte);
    }
}
