/*
 * LCD_Program.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Mohamed Tapl
 *
 *  Updated: complete Lcd_4BitMode implementation.
 */



#include "LCD_Interface.h"
#include <util/delay.h>

/*----- Helper: pulse Enable pin -----*/
static void LCD_PulseEnable(void)
{
    DIO_WriteForPin(EGroup, EPin, DIO_High);
    _delay_us(1);
    DIO_WriteForPin(EGroup, EPin, DIO_Low);
    _delay_us(100);
}

/*----- 4-bit nibble send (high nibble of data) -----*/
#if LCD_Mode == Lcd_4BitMode
static void LCD_Send4Bits(uint8_t Copy_u8Nibble)
{
    /* Copy_u8Nibble bits 7..4 go to D7..D4 pins */
    DIO_WriteForPin(D7Group, D7Pin, ReadBit(Copy_u8Nibble, 7));
    DIO_WriteForPin(D6Group, D6Pin, ReadBit(Copy_u8Nibble, 6));
    DIO_WriteForPin(D5Group, D5Pin, ReadBit(Copy_u8Nibble, 5));
    DIO_WriteForPin(D4Group, D4Pin, ReadBit(Copy_u8Nibble, 4));
    LCD_PulseEnable();
}
#endif

void LCD_Init()
{
    #if LCD_Mode == Lcd_8BitMode
        DIO_DirectionSelectForPin(RSGroup, RSPin, DIO_Output);
        DIO_DirectionSelectForPin(RWGroup, RWPin, DIO_Output);
        DIO_DirectionSelectForPin(EGroup, EPin, DIO_Output);
        DIO_DirectionSelectForGroup(DataGroup, DIO_AllHigh);
        /*Init*/
        _delay_ms(35);
        LCD_SendCommand(Lcd_FunctionSet);
        _delay_ms(1);
        LCD_SendCommand(Lcd_DisplayOnOff);
        _delay_ms(1);
        LCD_SendCommand(Lcd_ClearScreen);
        _delay_ms(2);
        LCD_SendCommand(Lcd_EntryMode);
    #elif LCD_Mode == Lcd_4BitMode
        /* Direction: control pins */
        DIO_DirectionSelectForPin(RSGroup, RSPin, DIO_Output);
        DIO_DirectionSelectForPin(RWGroup, RWPin, DIO_Output);
        DIO_DirectionSelectForPin(EGroup, EPin, DIO_Output);
        /* Direction: data nibble pins */
        DIO_DirectionSelectForPin(D4Group, D4Pin, DIO_Output);
        DIO_DirectionSelectForPin(D5Group, D5Pin, DIO_Output);
        DIO_DirectionSelectForPin(D6Group, D6Pin, DIO_Output);
        DIO_DirectionSelectForPin(D7Group, D7Pin, DIO_Output);

        /* RW always low (write mode) */
        DIO_WriteForPin(RWGroup, RWPin, DIO_Low);

        /* Power-on delay */
        _delay_ms(40);

        /* HD44780 4-bit initialization sequence */
        DIO_WriteForPin(RSGroup, RSPin, DIO_Low);

        /* Send 0x30 three times (Function Set in 8-bit bootstrap) */
        LCD_Send4Bits(0x30);
        _delay_ms(5);
        LCD_Send4Bits(0x30);
        _delay_us(150);
        LCD_Send4Bits(0x30);
        _delay_us(150);

        /* Switch to 4-bit mode */
        LCD_Send4Bits(0x20);
        _delay_us(150);

        /* Now in 4-bit mode — full byte commands work */
        LCD_SendCommand(Lcd_FunctionSet);     /* 0x28: 4-bit, 2-line, 5×8 */
        _delay_ms(1);
        LCD_SendCommand(Lcd_DisplayOnOff);    /* 0x0C: display on, cursor off */
        _delay_ms(1);
        LCD_SendCommand(Lcd_ClearScreen);     /* 0x01 */
        _delay_ms(2);
        LCD_SendCommand(Lcd_EntryMode);       /* 0x06: increment, no shift */
    #else
            #error "Invalid Lcd Mode"
    #endif
}

void LCD_SendCommand(uint8_t Command)
{
    #if LCD_Mode == Lcd_8BitMode
        DIO_WriteForPin(RSGroup, RSPin, DIO_Low);
        DIO_WriteForPin(RWGroup, RWPin, DIO_Low);
        DIO_WriteForGroup(DataGroup, Command);
        DIO_WriteForPin(EGroup, EPin, DIO_High);
        _delay_ms(1);
        DIO_WriteForPin(EGroup, EPin, DIO_Low);
    #elif LCD_Mode == Lcd_4BitMode
        DIO_WriteForPin(RSGroup, RSPin, DIO_Low);
        DIO_WriteForPin(RWGroup, RWPin, DIO_Low);
        /* Send high nibble first, then low nibble */
        LCD_Send4Bits(Command);
        LCD_Send4Bits((uint8_t)(Command << 4));
        _delay_us(50);
    #else
            #error "Invalid Lcd Mode"
    #endif

}

void LCD_WriteCharacter(uint8_t Character)
{
    #if LCD_Mode == Lcd_8BitMode
        DIO_WriteForPin(RSGroup, RSPin, DIO_High);
        DIO_WriteForPin(RWGroup, RWPin, DIO_Low);
        DIO_WriteForGroup(DataGroup, Character);
        DIO_WriteForPin(EGroup, EPin, DIO_High);
        _delay_ms(1);
        DIO_WriteForPin(EGroup, EPin, DIO_Low);
    #elif LCD_Mode == Lcd_4BitMode
        DIO_WriteForPin(RSGroup, RSPin, DIO_High);
        DIO_WriteForPin(RWGroup, RWPin, DIO_Low);
        LCD_Send4Bits(Character);
        LCD_Send4Bits((uint8_t)(Character << 4));
        _delay_us(50);
    #else
        #error "Invalid Lcd Mode"
    #endif

}

void LCD_WriteString(uint8_t *String)
{
    if(String != NULL)
    {
        uint8_t Index = 0;
        while(String[Index] != NullChar)
        {
            LCD_WriteCharacter(String[Index]);
            Index++;
        }
    }
}

void LCD_MoveTo(uint8_t Line, uint8_t Digit)
{
    uint8_t DDRAM_Address = 0;
    switch(Line)
    {
        case Lcd_Line1: DDRAM_Address = Lcd_Line1Address + Digit; break;
        case Lcd_Line2: DDRAM_Address = Lcd_Line2Address + Digit; break;
        default: break;
    }
    LCD_SendCommand(Lcd_SetDDRAMCommand | DDRAM_Address);
}

void LCD_StoreSpecialCharacter(uint8_t * SpecialCharacter, uint8_t Location)
{
    if(SpecialCharacter == NULL || Location > Lcd_CGRAMMaxLocation)
    {
        return;
    }

    uint8_t LocationAddress = Location * Lcd_CGRAMMaxSizeofLocation;
    LCD_SendCommand(Lcd_SetCGRAMCommand | LocationAddress);
    uint8_t counter = 0;
    for(counter = 0; counter < Lcd_CGRAMMaxSizeofLocation; counter++)
    {
        LCD_WriteCharacter(SpecialCharacter[counter]);
    }
    LCD_MoveTo(Lcd_Line1, 0);

}

void LCD_WriteNumber(int32_t Number)
{
    uint8_t NumberDigits[10] = {0};
    uint8_t Index = 0;
    int8_t Counter = 0;
    if(Number == 0)
    {
        LCD_WriteCharacter('0');
        return;
    }
    if(Number < 0)
    {
       LCD_WriteCharacter('-');
       Number = Number * -1;
    }
    while(Number != 0)
    {
        NumberDigits[Index] = (Number % 10) + '0';
        Number = Number / 10;
        Index++;
    }
    for(Counter = Index - 1; Counter >= 0; Counter--)
    {
        LCD_WriteCharacter(NumberDigits[Counter]);
    }
}
