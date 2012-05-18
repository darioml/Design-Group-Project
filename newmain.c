#include <stdio.h>
#include <pic.h>
#include <htc.h>
#include <time.h>
// Using Internal Clock of 20 Mhz
//#define FOSC 800000


//Bug variables
#define	BUGMOTOR_R RC6
#define	BUGMOTOR_L RC7

#define	BUGSENSOR_R RC4
#define	BUGSENSOR_L RC5



__CONFIG(FOSC_INTOSC & WDTE_OFF & CLKOUTEN_ON);

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



void _delay_ms(unsigned int ms)
{
    do
    {
        for (int i = 0; i < 5; i++)
        {
            NOP();
        }
    } while(--ms);
}

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
// Function to control the buzzers
// *************************************************
/*
 * This makes four beeps - the first three last 0.2 seconds and are 0.7 seconds apart
 * (Meaning 0.9 / beep cycle). The last beep is a higher frequency, and means "GO"
 * This lasts 0.3 seconds, and has no post-delay, as we want to start the bug asap
 * after the sound.
 */

void doDelay()
{
    RC2 = 0;
    _delay_ms(500); //sleep for 1 second
    RC2 = 1;
    _delay_ms(500); //sleep for 1 second

    RC2 = 0;
    _delay_ms(500); //sleep for 1 second
    RC2 = 1;
    _delay_ms(500); //sleep for 1 second

    RC2 = 0;
    _delay_ms(500); //sleep for 1 second
    RC2 = 1;
    _delay_ms(500); //sleep for 1 second
    RC2 = 0;

    _delay_ms(1000);

    //From here on, we need to do the buzzer s in a nice timespace, while keeping 1 seconds
    //for each LCD countdown.
    for (int i = 0; i < 80; i++)
    {
        RC3 = 1;
        _delay_ms(4);
        RC3 = 0;
        _delay_ms(4);
    }
    _delay_ms(360);


    for (int i = 0; i < 80; i++)
    {
        RC3 = 1;
        _delay_ms(4);
        RC3 = 0;
        _delay_ms(4);
    }
    _delay_ms(360);

    for (int i = 0; i < 80; i++)
    {
        RC3 = 1;
        _delay_ms(4);
        RC3 = 0;
        _delay_ms(4);
    }
    _delay_ms(360);
    
    _delay_ms(200);
    
    for (int i = 0; i < 400; i++)
    {
        RC3 = 1;
        _delay_ms(1);
        RC3 = 0;
        _delay_ms(1);
    }
    

    //lcd_goto(0);	// select first line
    //lcd_puts("Start!");
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
    TRISC = 0b00110000;    //This is speaker & Motor outputs, and inputs for a/D

    PORTA = 0x00; //Clear all pins.
    PORTB = 0x00;
    PORTC = 0x00;

    doDelay();      //does LCD and Buzzer at the same time..
    
    ADCInit ();     //Let's fire up the ADC!

    int leftpt, rightpt, read;
    while (1) //let's continuously loop this, since it's controling our motor!
    {
        //test for left phototransistor
        read = readchannel(1);
        leftpt = (read > 80) ? 1 : 0;

        //test for right phototransistor
        read = readchannel(0);
        rightpt = (read > 80) ? 1 : 0;

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

