#include <stdio.h>
#include <pic.h>
//#include <as16f917.h>
#include <htc.h>
#include <time.h>
#include "newfile.c"
//#include <>
// Using Internal Clock of 8 Mhz
#define FOSC 8000000L


__CONFIG(FOSC_INTOSCCLK & WDTE_OFF);

// *************************************************
// Global Variables
// *************************************************
time_t start, stop;
int clock_t, ticks; long count;
int lastMotor;

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
void ADCInit(int ADC_Channel);
unsigned int ADCRead(unsigned char ch);
void InitialiseADC (unsigned char ADC_Channel);
unsigned int ReadADC(unsigned char ADC_Channel);



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
        {		//if left sensor is on track, turn on right motor
            //outputHighPin (RC1); // RC1 is right motor
            RC1 = 1;
	}
        else
        {		//if left sensor is off track, switch off right motor
            //outputLowPin (RC1);
            RC1 = 0;
            lastMotor = 2;
	}


	if (Rightpt == 1)
        {		//similarly, we do the same for the right motor
            //outputHighPin (RC0); //RC0 is left motor
            RA0 = 1;
	}
        else
        {
            //outputLowPin (RC0);
            RA0 = 0;
            lastMotor = 1;
        }
}


// *************************************************
// Function to Search for the Track if the both sensors are off the track.
// Used to cross the intersection in this way.
// *************************************************
int searchTrack(void)
{
	if (lastMotor == 1)
	{
            controlMotor(1, 0);
	}
        else if (lastMotor == 2)
	{
            controlMotor(0, 1);
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

int controlBuzzer()
{


}


//Function to Initialise the ADC Module
void ADCInit(int ADC_Channel)
{
   //We use default value for +/- Vref

   //VCFG0=0,VCFG1=0
   //That means +Vref = Vdd (5v) and -Vref=GEN

   //Port Configuration
   //We also use default value here too
   //All ANx channels are Analog

   /*
      ADCON2

      *ADC Result Right Justified.
      *Acquisition Time = 2TAD
      *Conversion Clock = 32 Tosc
   */

    if      (ADC_Channel == 0) 	TRISA0 = 1;
	else if (ADC_Channel == 1) 	TRISA1 = 1;
	else if (ADC_Channel == 2) 	TRISA2 = 1;
	else if	(ADC_Channel == 3) 	TRISA3 = 1;

        // Do we want to define this at the start of main???????????
        //?!??!?!?!??!?!??!?!
        // !?=?!?!?!?!?

/* 	------
	Specify all to be analogue inputs, refered to VDD.
	Please refer to Page 118 from PIC manual for other configurations.
 ------	*/

	/* Analogue-RA0/RA1/RA3 Digital-RA2/RA5	*/
	ADCON1	= 0b10000100;
}

//Function to Read given ADC channel (0-13)
unsigned int ADCRead(unsigned char ch)
{
   ADCON0 = 0x00; //reset adcon, which is the main settings of the "A/D" conversion
   ADCON0 = (ch<<2);   //Select ADC Channel

   ADON=1;  //switch on the adc module

   GO_DONE=1;//Start conversion
   while(GO_DONE); //wait for the conversion to finish

   ADON=0;  //switch off adc

   return (ADRESH<8)|ADRESL;
}


/*************************************************************
	Initialising the ADC channels
**********************************************************/
void InitialiseADC (unsigned char ADC_Channel)
{
	if      (ADC_Channel == 0) 	TRISA0 = 1;
	else if (ADC_Channel == 1) 	TRISA1 = 1;
	else if (ADC_Channel == 2) 	TRISA2 = 1;
	else if	(ADC_Channel == 3) 	TRISA3 = 1;

/* 	------
	Specify all to be analogue inputs, refered to VDD.
	Please refer to Page 118 from PIC manual for other configurations.
 ------	*/

	/* Analogue-RA0/RA1/RA3 Digital-RA2/RA5	*/
	ADCON1	= 0b10000100;
}

/*************************************************************
	Reads the ADC level input on a specified ADC channel.
	Takes in an 10-bit ADC channel number.
	Returns an 10 bit number that signifies this level.
**********************************************************/
unsigned int ReadADC(unsigned char ADC_Channel)
{
    volatile unsigned int ADC_VALUE;

    ADON = 1;
    /* Selecting ADC channel */
    ADCON0 = (ADC_Channel << 3) + 1;	/* Enable ADC, Fosc/2 */

    ADIE     =	0;									 	 		 	/* Masking the interrupt */
    ADIF     =      0;                  /* Resetting the ADC interupt bit */
    ADRESL   =	0;              /* Resetting the ADRES value register */
    ADRESH   =	0;

  //ADGO = 1;				  					  		 	/* Staring the ADC process */
  //ADCON0[2] = 1;
        while(!ADIF) continue;			   /* Wait for conversion complete */

	ADC_VALUE	=	 ADRESL;										/* Getting HSB of CCP1 */
	ADC_VALUE	+= (ADRESH << 8);		 				  /* Getting LSB of CCP1 */

  return (ADC_VALUE);     /* Return the value of the ADC process */
}


// *************************************************
// Main function, contains loop to read the values from PhotoTransistor
// Call functions to do Analogue to digital conversion
// Call functions to searchTrack or controlMotor
// *************************************************

int main(void)
{
    //OSCCON=0x70;         // Select 8 Mhz internal clock
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0X00;
    TRISE = 0X00;    // Set All on PORT B,C,D,E  as Output

    _delay_ms(8000);

    InitialiseADC (0);   // initialise channel   left transistor  /* Analogue-RA0/RA1/RA3 Digital-RA2/RA5	*/
    InitialiseADC (1);
    int read, leftpt, rightpt;

    while (1) //let's continuously loop this, since it's controling our motor!
    {
        /*test for left phototransistor*/
        read = ReadADC(0);// get the input of analoge and return digital value of 10 bits, A2
        leftpt = (read > 1.5) ? 1 : 0;

        /*test for right phototransistor */
        read = ReadADC(1);  // get the input of analoge and return digital value of 10 bits, A2D
        rightpt = (read > 1.5) ? 1 : 0;

        if((leftpt==0) && (rightpt==0))
        {
            searchTrack();
        }
        else
        {
            controlMotor(leftpt, rightpt);
        }
    }
}

