/*
 *	LCD interface example
 *	Uses routines from delay.c
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 bit mode, with
 *	the hardware connected as follows (the standard 14 pin 
 *	LCD connector is used):
 *	
 *	PORTD bits 0-3 are connected to the LCD data bits 4-7 (high nibble)
 *	PORTA bit 3 is connected to the LCD RS input (register select)
 *	PORTA bit 1 is connected to the LCD EN bit (enable)
 *	
 *	To use these routines, set up the port I/O (TRISA, TRISD) then
 *	call lcd_init(), then other routines as required.
 *	
 */

#ifndef _XTAL_FREQ
 // Unless specified elsewhere, 4MHz system frequency is assumed
 #define _XTAL_FREQ 1700000L
#endif


#include	<htc.h>
#include	"lcd2.h"

#define	LCD_RS RA0
#define	LCD_RW RA1
#define LCD_EN RA2

#define LCD_DATA	PORTB

#define	LCD_STROBE()	((LCD_EN = 1),(LCD_EN=0))

/* write a byte to the LCD in 4 bit mode */

#define ___delay_us(x) { unsigned char us; \
	  	       us = (x)/(12000000/_XTAL_FREQ)|1; \
		       while(--us != 0) continue; }

void ___delay_ms(unsigned int ms)
{
    unsigned char i;
    do
    {
        i = 4;
        do
        {
            ___delay_us(250);
        } while(--i);
    } while(--ms);
}

void
lcd_write(unsigned char c)
{
	___delay_us(40);
	LCD_DATA = ( ( (c<<4) | 0x00 ) & LCD_DATA );
	LCD_STROBE();
}

/*
 * 	Clear and home the LCD
 */

void SendIns(unsigned char c)
{
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_DATA = ( c );
    ___delay_ms(5);
    LCD_EN = 1;
    ___delay_ms(5);
    LCD_EN = 0;
    ___delay_ms(5);
}

void SendDat(unsigned char c)
{
    LCD_RW = 0;
    LCD_DATA = ( c ); 
    LCD_RS = 1;
    ___delay_us(10);
    LCD_EN = 1;
    ___delay_ms(5);
    LCD_EN = 0;
    LCD_RS = 0;
    ___delay_ms(5);
}

void initial()
{

}

void
lcd_clear(void)
{
	LCD_RS = 0;
	lcd_write(0x1);
	___delay_ms(2);
}

/* write a string of chars to the LCD */

void
lcd_puts(const char * s)
{
	LCD_RS = 1;	// write characters
	while(*s)
		lcd_write(*s++);
}

/* write one character to the LCD */

void
lcd_putch(char c)
{
	LCD_RS = 1;	// write characters
	lcd_write( c );
}


/*
 * Go to the specified position
 */

void
lcd_goto(unsigned char pos)
{
	LCD_RS = 0;
	lcd_write(0x80+pos);
}
	
/* initialise the LCD - put into 4 bit mode */
void
lcd_init()
{
        ___delay_ms(40);

	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;

        //Display ON/OFF
        //SendIns(0b00001000);
	//___delay_us(100);

        //function set - 8 bit and 2 line
	SendIns(0b00111000);
        ___delay_us(100);

        //Display ON/OFF, blink on
        SendIns(0b00001111);
	___delay_us(100);

        //Display CLEAR
        SendIns(0b00000001);
	___delay_ms(5);

        //Entry Mode Set
        SendIns(0b00000111);
	___delay_ms(5);
        
        /*___delay_ms(40);
        
	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;

        //function set
	SendIns(0b00111000);
        ___delay_us(100);
	SendIns(0b00111000);
	___delay_us(100);

        //Display ON/OFF
        SendIns(0b00001111);
	___delay_us(100);

        //Display CLEAR
        SendIns(0b00000001);
	___delay_ms(5);

        //Entry Mode Set
        SendIns(0b00000110);
	___delay_ms(5);

        //Go Home
        SendIns(0b00000010);
	___delay_ms(5);*/
}
