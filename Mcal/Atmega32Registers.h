/*
 * Atmega32Registers.h
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 */

#ifndef MCAL_ATMEGA32REGISTERS_H_
#define MCAL_ATMEGA32REGISTERS_H_
#include <stdint.h>
#define _SetAddress8bit(Addrress)   *((volatile uint8_t*)Addrress)
#define _SetAddress16bit(Addrress)  *((volatile uint16_t*)Addrress)
#define _SetAddress32bit(Addrress)  *((volatile uint32_t*)Addrress)


#define DDRA_Reg    _SetAddress8bit(0x3A)
#define PORTA_Reg   _SetAddress8bit(0x3B)
#define PINA_Reg    _SetAddress8bit(0x39)

#define DDRB_Reg    _SetAddress8bit(0x37)
#define PORTB_Reg   _SetAddress8bit(0x38)
#define PINB_Reg    _SetAddress8bit(0x36)

#define DDRC_Reg    _SetAddress8bit(0x34)
#define PORTC_Reg   _SetAddress8bit(0x35)
#define PINC_Reg    _SetAddress8bit(0x33)

#define DDRD_Reg    _SetAddress8bit(0x31)
#define PORTD_Reg   _SetAddress8bit(0x32)
#define PIND_Reg    _SetAddress8bit(0x30)

#define SREG_Reg    _SetAddress8bit(0x5F)

#define MCUCR_Reg    _SetAddress8bit(0x55)
#define MCUCSR_Reg   _SetAddress8bit(0x54)
#define GICR_Reg     _SetAddress8bit(0x5B)
#define GIFR_Reg     _SetAddress8bit(0x5A)


#define ADMUX_Reg    _SetAddress8bit(0x27)
#define ADCSRA_Reg   _SetAddress8bit(0x26)
#define ADCH_Reg     _SetAddress8bit(0x25)
#define ADCL_Reg     _SetAddress8bit(0x24)
#define ADCData_Reg  _SetAddress16bit(0x24)
#define SFIOR_Reg    _SetAddress8bit(0x50)

#define TCCR0_Reg    _SetAddress8bit(0x53)
#define TCNT0_Reg    _SetAddress8bit(0x52)
#define OCR0_Reg     _SetAddress8bit(0x5C)

#define TCCR2_Reg    _SetAddress8bit(0x45)
#define TCNT2_Reg    _SetAddress8bit(0x44)
#define OCR2_Reg     _SetAddress8bit(0x43)

#define TIMSk_Reg    _SetAddress8bit(0x59)
#define TIFR_Reg     _SetAddress8bit(0x58)

/* ---- Timer1 Registers ---- */
#define TCCR1A_Reg   _SetAddress8bit(0x4F)
#define TCCR1B_Reg   _SetAddress8bit(0x4E)
#define TCNT1_Reg    _SetAddress16bit(0x4C)
#define OCR1A_Reg    _SetAddress16bit(0x4A)
#define OCR1B_Reg    _SetAddress16bit(0x48)
#define ICR1_Reg     _SetAddress16bit(0x46)

/* ---- UART Registers ---- */
#define UDR_Reg       _SetAddress8bit(0x2C)
#define UCSRA_Reg     _SetAddress8bit(0x2B)
#define UCSRB_Reg     _SetAddress8bit(0x2A)
#define UCSRC_Reg     _SetAddress8bit(0x40)
#define UBRRH_Reg     _SetAddress8bit(0x40)
#define UBRRL_Reg     _SetAddress8bit(0x29)

/* ---- TWI Registers ---- */
#define TWBR_Reg_G   _SetAddress8bit(0x20)
#define TWSR_Reg_G   _SetAddress8bit(0x21)
#define TWAR_Reg_G   _SetAddress8bit(0x22)
#define TWDR_Reg_G   _SetAddress8bit(0x23)
#define TWCR_Reg_G   _SetAddress8bit(0x56)

/* ---- SPI Registers ---- */
#define SPCR_Reg     _SetAddress8bit(0x2D)
#define SPSR_Reg     _SetAddress8bit(0x2E)
#define SPDR_Reg     _SetAddress8bit(0x2F)

#endif /* MCAL_ATMEGA32REGISTERS_H_ */
