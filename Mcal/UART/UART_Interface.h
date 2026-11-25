/*
 * UART_Interface.h
 *
 *  Created on: Nov 10, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_

#include <stdint.h>

void UART_voidInit(void);
void UART_voidSendData(uint8_t Copy_u8Data);
uint8_t UART_u8ReceiveData(void);
void UART_voidSendString(const char* Copy_pcString);

/* RX Complete Interrupt Enable/Disable and Callback */
void UART_voidEnableRXCInterrupt(void);
void UART_voidDisableRXCInterrupt(void);
void UART_voidSetRXCCallBack(void (*Copy_pvCallBack)(uint8_t));

#endif /* MCAL_UART_UART_INTERFACE_H_ */
