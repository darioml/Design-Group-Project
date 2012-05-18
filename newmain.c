#include <stdio.h>
#include <pic.h>
#include <htc.h>
#include <time.h>
//#include "lcd.h"
#include "lcd2.h"
// Using Internal Clock of 20 Mhz
#define FOSC 50000L

//Bug variables
#define	BUGMOTOR_R RC6
#define	BUGMOTOR_L RC7

#define	BUGSENSOR_R RC4
#define	BUGSENSOR_L RC5



__CONFIG(FOSC_INTOSC & WDTE_OFF);

// *************************************************
// Global Variables
// *************************************************
time_t start, stop;
int clock_t, ticks; long count;
int lastMotor;
int ignore;

/*
 function declerations
 */
void _delay_ms(unsigned int ms);
void outputHighPin (bit pin);
void outputLowPin (bit pin);
void controlMotor(int Leftpt, int Rightpt);
int searchTrack(void);
int controlLCD();
int controlBuzzer();
void ADCInit();
unsigned int ADCRead(unsigned char ch);
void InitialiseADC (unsigned char ADC_Channel);
int readchannel(int chan);
unsigned int ReadADC(unsigned char ADC_Channel);
void lcdCountdown(char t);



// *************************************************
// Delay Function
// *************************************************
#define _delay_us(x) { unsigned char us; \
	  	       us = (x)/(12000000/FOSC)|1; \
		       while(--us != 0) continue; }

void _delay_ms(unsigned int ms)
{
    unsigned char i;
    do
    {
        i = 4;
        do
        {
            _delay_us(250);
        } while(--i);
    } while(--ms);
}

/* CHAR
void outputHighPin (char pin)
{
    pin = 1;
}
void outputLowPin (char pin)
{
    pin = 0;
}*/

// *************************************************
// Function to control the motor
// Added function to help keep the bug on track
// *************************************************

void controlMotor(int Leftpt, int Rightpt)
{
    if (Leftpt == 1)
    {
        //if left sensor is on track, turn on right motor
        //outputHighPin (RC6); // RC6 is right motor
        BUGMOTOR_R = 1;
    }
    else
    {
        //if left sensor is off track, switch off right motor
        //outputLowPin (RC6);
        BUGMOTOR_R = 0;
        if (ignore == 0)
        {
            lastMotor = 1;
        }
    }


    if (Rightpt == 1)
    {
        //similarly, we do the same for the right motor
        //outputHighPin (RC7); //RC7 is left motor
        BUGMOTOR_L = 1;
    }
    else
    {
        //outputLowPin (RC7);
        BUGMOTOR_L = 0;
        if (ignore == 0)
        {
            lastMotor = 2;
        }
    }
}


// *************************************************
// Function to Search for the Track if the both sensors are off the track.
// Used to cross the intersection in this way.
// *************************************************
int searchTrack(void)
{
    ignore = 1;
    if (lastMotor == 2)
    {
        controlMotor(1, 0);
    }
    else if (lastMotor == 1)
    {
        controlMotor(0, 1);
    }
    else if (lastMotor == 0)
    {
        controlMotor(1,1);
    }
}





// *************************************************
// Function to control the LCD display
// *************************************************
int controlLCD()
{

}

// *************************************************
// Function to control the buzzers
// *************************************************
/*
 * This makes four beeps - the first three last 0.2 seconds and are 0.7 seconds apart
 * (Meaning 0.9 / beep cycle). The last beep is a higher frequency, and means "GO"
 * This lasts 0.3 seconds, and has no post-delay, as we want to start the bug asap
 * after the sound.
 */

int controlBuzzer()
{
    //how many buzzers do we want? I'll say we have two, since we can always change that
    RC4 = 1;
    _delay_ms(200); //sleep for .2 seconds
    RC4 = 0;
    _delay_ms(700); //sleep for .7 seconds

    RC4 = 1;
    _delay_ms(700); //sleep for .2 seconds
    RC4 = 0;
    _delay_ms(700); //sleep for .7 seconds

    RC4 = 1;
    _delay_ms(200); //sleep for .2 seconds
    RC4 = 0;
    _delay_ms(700); //sleep for .7 seconds

    RC5 = 1;
    _delay_ms(300); //sleep for .3 seconds
    RC5 = 0;

}

void doDelay()
{
    RC4 = 0;
    RC5 = 0;

    lcdCountdown('8');
    _delay_ms(1000); //sleep for 1 second

    lcdCountdown('7');
    _delay_ms(1000); //sleep for 1 second

    lcdCountdown('6');
    _delay_ms(1000); //sleep for 1 second

    lcdCountdown('5');
    _delay_ms(1000); //sleep for 1 second

    lcdCountdown('4');
    _delay_ms(1000);

    //From here on, we need to do the buzzer s in a nice timespace, while keeping 1 seconds
    //for each LCD countdown.
    lcdCountdown('3');
    RC4 = 1;
    _delay_ms(200);
    RC4 = 0;
    _delay_ms(700);

    RC4 = 1;
    _delay_ms(100);
    lcdCountdown('2');
    _delay_ms(100);
    RC4 = 0;
    _delay_ms(700);

    RC4 = 1;
    _delay_ms(200);
    lcdCountdown('1');
    RC4 = 0;
    _delay_ms(700);

    RC5 = 1; //this is the higher pitch sound
    _delay_ms(300);
    RC5 = 0;

    //lcd_goto(0);	// select first line
    //lcd_puts("Start!");
}

void lcdCountdown(char t)
{
    //lcd_goto(0); //go to the first field
    //lcd_putch(t);
}


//Function to Initialise the ADC Module
void ADCInit()
{
    ANSELC = 0b00110000;
    ADCON1 = 0x00;
    //ANSEL = 0b11111111; //Make all analogue inputs accept analogue
                        //We only use inputs for the phototransistors, so nothing else is affected
    //ADCON1	= 0b10000100; this is for the clock, but it's fine as it is per default!
}

//Function to Read given ADC channel (0-13) (Written with basebone of online code, and pic16f917 datasheet)
unsigned int ADCRead(unsigned char ch)
{

   ADCON0 = 0x00; //reset adcon, which is the main settings of the "A/D" conversion
   ADCON0 = (ch<<2);   //Select ADC Channel

   ADON=1;  //switch on the adc module

   GO_nDONE=1;//Start conversion
   while(GO_nDONE); //wait for the conversion to finish

   ADON=0;  //switch off adc

   return ADRESH; //10 bits are given back over two memories - This will give us our stuff, I think.
   //Will c turn the bits into an int? should do..
}


/*************************************************************
	Initialising the ADC channels
*********************************************************
void InitialiseADC (unsigned char ADC_Channel)
{
    //THIS FUNCTION IS OBSOLETE, USE ADCINIT
 *
 *
 *
	if      (ADC_Channel == 0) 	TRISA0 = 1;
	else if (ADC_Channel == 1) 	TRISA1 = 1;
	else if (ADC_Channel == 2) 	TRISA2 = 1;
	else if	(ADC_Channel == 3) 	TRISA3 = 1;*/

/* 	------
	Specify all to be analogue inputs, refered to VDD.
	Please refer to Page 118 from PIC manual for other configurations.
 ------	*/

	/* Analogue-RA0/RA1/RA3 Digital-RA2/RA5
	ADCON1	= 0b10000100;
}*/

/*************************************************************
	Reads the ADC level input on a specified ADC channel.
	Takes in an 10-bit ADC channel number.
	Returns an 10 bit number that signifies this level.
*********************************************************
unsigned int ReadADC(unsigned char ADC_Channel)
{
    //THIS FUNCTION IS OBSOLETE, USE ADCREAD
 *
 *
    volatile unsigned int ADC_VALUE;

    ADON = 1;*/
    /* Selecting ADC channel
    ADCON0 = (ADC_Channel << 3) + 1;	// Enable ADC, Fosc/2

    ADIE     =	0;		 	// Masking the interrupt
    ADIF     =      0;                  //Resetting the ADC interupt bit
    ADRESL   =	0;              // Resetting the ADRES value register
    ADRESH   =	0;

  //ADGO = 1;				  // Staring the ADC process
  //ADCON0[2] = 1;
        while(!ADIF) continue;			   // Wait for conversion complete

	ADC_VALUE	=	 ADRESL;			// Getting HSB of CCP1
	ADC_VALUE	+= (ADRESH << 8);			  // Getting LSB of CCP1

  return (ADC_VALUE);     /* Return the value of the ADC process
}
*/

// *************************************************
// Main function, contains loop to read the values from PhotoTransistor
// Call functions to do Analogue to digital conversion
// Call functions to searchTrack or controlMotor
// *************************************************

int readchannel(int chan)
{
    if (chan == 0) //A/D AN5 RE0
    {
        ADCON0 = 0b01000001; // Read RC4 AN16

        GO_nDONE=1;//Start conversion
        while(GO_nDONE); //wait for the conversion to finish

        ADON=0;  //switch off adc

        return ADRESH;
    }
    else if (chan == 1) //A/D AN6 RE1
    {
        ADCON0 = 0b01000101; // Read RC5 AN17

        GO_nDONE=1;//Start conversion
        while(GO_nDONE); //wait for the conversion to finish

        ADON=0;  //switch off adc

        return ADRESH;
    }
}

int main(void)
{
    //OSCCON=0x70;         // Select 8 Mhz internal clock
    TRISA = 0x00;    //This is for the LCD EN, RW and RS bits
    TRISB = 0x00;    //This is for the LCD Data!
    TRISC = 0b00110000;    //This is buzzer & Motor outputs, and inputs for a/D

    PORTA = 0x00; //Clear all pins.
    PORTB = 0x00;
    PORTC = 0x00;

    //LATA = 0x00;
    //CMCON0 = 0x07;
    //LCDCON = 0x00;
    //ANSELA = 0x00;

    //lcd_init();
    //SendDat(0b010000001);


    //doDelay();      //does LCD and Buzzer at the same time..
    
    ADCInit ();     //Let's fire up the ADC!

    //CGRAM and DDRAM
    //SendIns(0b01000000);
    //_delay_ms(5);
    //SendIns(0b10000000);
    //_delay_ms(2000);

    //RA2 = 0;
    //PORTB = 0b00010100; //shift cursor to the right
    //RA1 = 0;
    //RA0 = 1;
    //_delay_ms(5);
    //RA2 = 1;
    //_delay_ms(5);
    //RA2 = 0;
    //RA0 = 0;
    //_delay_ms(5);

    

    int leftpt, rightpt, read;
    while (1) //let's continuously loop this, since it's controling our motor!
    {
        //test for left phototransistor
        //read = ADCRead(0);// get the input of analoge and return digital value of 10 bits, A2
        read = readchannel(1);
        leftpt = (read > 90) ? 1 : 0;

        //test for right phototransistor
        //read = ADCRead(1);  // get the input of analoge and return digital value of 10 bits, A2D
        read = readchannel(0);
        rightpt = (read > 115) ? 1 : 0;


        //_delay_ms(3000);

        if((leftpt==0) && (rightpt==0))
        {
            searchTrack();
        }
        else
        {
            ignore = 0;
            controlMotor(leftpt, rightpt);
        }
    }

}

