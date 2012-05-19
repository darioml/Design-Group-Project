#include <stdio.h>
#include <pic.h>
#include <htc.h>
#include <time.h>

//Bug variables
#define	BUGMOTOR_R RC6
#define	BUGMOTOR_L RC7

#define	BUGSENSOR_R RC4
#define	BUGSENSOR_L RC5

#define BUGSPEAKER RC3



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
void controlMotor(int rightmotor, int leftmotor);
int searchTrack(void);
void ADCInit();
int readchannel(int chan);
void doDelay();



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

void controlMotor(int rightmotor, int leftmotor)
{
    if (rightmotor == 1)
    {
        BUGMOTOR_R = 1;
    }
    else
    {
        BUGMOTOR_R = 0;
        lastMotor = 1;
    }


    if (leftmotor == 1)
    {
        BUGMOTOR_L = 1;
    }
    else
    {
        BUGMOTOR_L = 0;
        lastMotor = 2;
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
    // 0 seconds
    for (int i = 0; i < 12; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(8);
        BUGSPEAKER = 0;
        _delay_ms(8);
    }
    _delay_ms(808); //sleep for 1 second

    // 1 second

    for (int i = 0; i < 12; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(8);
        BUGSPEAKER = 0;
        _delay_ms(8);
    }
    _delay_ms(808); //sleep for 1 second

    // 2 seconds

    for (int i = 0; i < 12; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(8);
        BUGSPEAKER = 0;
        _delay_ms(8);
    }
    _delay_ms(808); //sleep for 1 second

    // 3 seconds

    for (int i = 0; i < 12; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(8);
        BUGSPEAKER = 0;
        _delay_ms(8);
    }
    _delay_ms(808); //sleep for 1 second

    // 4 seconds

    //From here on, we need to do the speaker sounds in a nice timespace
    for (int i = 0; i < 80; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(4);
        BUGSPEAKER = 0;
        _delay_ms(4);
    }

    // 4s 640ms
    
    _delay_ms(426);

    // 5s 66ms

    for (int i = 0; i < 80; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(4);
        BUGSPEAKER = 0;
        _delay_ms(4);
    }

    // 5s 706ms
    
    _delay_ms(427);

    // 6s 133ms

    for (int i = 0; i < 80; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(4);
        BUGSPEAKER = 0;
        _delay_ms(4);
    }

    // 6s 773ms

    _delay_ms(427);

    // 7s 200ms
    
    for (int i = 0; i < 400; i++)
    {
        BUGSPEAKER = 1;
        _delay_ms(1);
        BUGSPEAKER = 0;
        _delay_ms(1);
    }

    // 8s
}

//Function to Initialise the ADC Module
void ADCInit()
{
    ANSELC = 0b00110000; //Make RC4 and RC5 analogue inputs
    ADCON1 = 0x00;       //Use the PIC clock for the A/D
}

int readchannel(int chan)
{
    if (chan == 0)
    {
        ADCON0 = 0b01000001; // Read RC4 AN16

        GO_nDONE=1;//Start conversion
        while(GO_nDONE); //wait for the conversion to finish

        ADON=0;  //switch off adc

        return ADRESH; //give back the first 8 bits, ignore the last two in ADRESL
    }
    else if (chan == 1)
    {
        ADCON0 = 0b01000101; // Read RC5 AN17

        GO_nDONE=1;//Start conversion
        while(GO_nDONE); //wait for the conversion to finish

        ADON=0;  //switch off adc

        return ADRESH; //give back the first 8 bits, ignore the last two in ADRESL
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
        leftpt = (read > 70) ? 1 : 0; //if it's on the white track, turn the right motor on

        //test for right phototransistor
        read = readchannel(0);
        rightpt = (read > 70) ? 1 : 0; //if it's on the white track, turn the left motor on

        if((leftpt==0) && (rightpt==0)) //if they are both of the track, find it!
        {
            searchTrack();
        }
        else
        {
            ignore = 0; //we want to store the last motor
            controlMotor(leftpt, rightpt); //Note: LeftPT controls RIGHT motor, and v.v.
        }
    }

}

