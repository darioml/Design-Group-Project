#include <stdio.h>
#include <pic.h>
//#include <as16f917.h>
#include <htc.h>
#include <time.h>
//#include <>
// Using Internal Clock of 8 Mhz
#define FOSC 8000000L



__CONFIG(FOSC_INTOSCCLK & WDTE_OFF);

// *************************************************
// Global Variables
// *************************************************
time_t start, stop;
int clock_t, ticks; long count;
int turnflag = 0;
int lastMotor = 0;


// *************************************************
// Delay Function
// *************************************************
#define _delay_us(x) { unsigned char us; \
	  	       us = (x)/(12000000/FOSC)|1; \
		       while(--us != 0) continue; }
void _delay_ms(unsigned int ms)
{
  unsigned char i;
  do {
    i = 4;
    do {
      _delay_us(250);
    } while(--i);
  } while(--ms);
}


// *************************************************
// Function to Search for the Track if the both sensors are off the track.
// Used to cross the intersection in this way.
// *************************************************
int searchTrack(void){

	//int lastMotor;		//0 if last motor switched off is leftMotor
	int leftMotor;			//1 to switch on left motor
	int rightMotor;			//1 to switch on right motor

	if (lastMotor == 1)
	{  //if the last motor switched off is the left motor, turn on left motor and off right motor
		leftMotor = 1;
		rightMotor = 0;
	} else if (lastMotor == 2)

	{ //if the last motor switched off is the right motor, turn on right motor and off left motor
		leftMotor = 0;
		rightMotor = 1;
	}
	turnflag = 0;
	//controlMotor(leftMotor, rightMotor);		//call controlMotor to control the motor.

}



// *************************************************
// Function to control the motor
// Added function to help keep the bug on track
// *************************************************
/*int controlMotor(Leftpt, Rightpt){

	//turnflag = 0 - not turning
	//turnflag = 1 - turning left
	//turnflag = 2 - turning right

	int timedifference, i;

	if (Leftpt == 1){		//if left sensor is on track, turn on right motor
		Output_high(//right motor pin); //the pins are like RA1, RB3, RC1

		if (turnflag == 2){		//if the bug was previously turning right, bring the bug to move straight on the track again by switching off the opposite motor for half the time
			turnflag = 0;
			time(&end);
			timedifference = difftime(stop, start);
			Output_low (left motor);
			lastMotor = 1;
			i=0;

			while(i< (timedifference / 2)){
				i++;
			}
		Output_high (left motor);
		}

	}	else {		//if left sensor is off track, switch off right motor
			Output_low (//right motor pin);
			lastMotor = 2;

			if (turnflag == 0){		//if the bug was not turning, record that it is now turning
				turnflag = 2;
				time(&start);
			}
	}


	if (Rightpt == 1){		//similarly, we do the same for the right motor
		Output_high(//left motor pin2);

		if (turnflag == 1){
			turnflag = 0;
			time(&end);
			timedifference = difftime(stop, start);
			Output_low (right motor);
			lastMotor = 2;
			i=0;

			while(i< (timedifference / 2)){
				i++;
			}
		Output_high (right motor);
		}

	}	else {
			Output_low(//left motor pin2);
			lastMotor = 1;

			if (turnflag == 0){
				turnflag = 1;
				time(&start);
			}
	}

}


*/
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

    /* Selecting ADC channel */
    ADCON0 = (ADC_Channel << 3) + 1;				 /* Enable ADC, Fosc/2 */

	ADIE	  =	0;									 	 		 	/* Masking the interrupt */
    ADIF 	  = 0;								 /* Resetting the ADC interupt bit */
	ADRESL	=	0; 						 /* Resetting the ADRES value register */
	ADRESH	=	0;

  //ADGO = 1;				  					  		 	/* Staring the ADC process */
  ADCON0[2] = 1;
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
    int x,y, leftpt, rightpt;

    while (1) //let's continuously loop this, since it's controling our motor!
    {
        /*test for left transistor*/
        x = ReadADC(0);// get the input of analoge and return digital value of 10 bits, A2

        if (x > 1.5){
            leftpt = 1;
        }
        else
        {
            leftpt = 0;
        }

        /*test for right transistor */

        y=ReadADC(1);  // get the input of analoge and return digital value of 10 bits, A2D

        if  (y > 1.5)
        {
            rightpt = 1;
        }
        else
        {
            rightpt = 0;
        }

        if((leftpt==0) && (rightpt==0))
        {
            //searchtrack();
        }
        else
        {
            //controlmotor(leftpt, rightpt);
        }
    }
}

