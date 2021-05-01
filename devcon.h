#ifndef DEVCON
#define DEVCON

#include <stdlib.h>
#include <math.h>
#include <i2c.h>
#include <delays.h>
#include <timers.h>

#include "simpletetris.h"
#include "ht16k33.h"
#include "helper.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = ON
#pragma config PBADEN = OFF

//LED1, LED2
#define LED1 PORTDbits.RD6
#define LED2 PORTDbits.RD7
//Buttons
#define ButtonLeft PORTBbits.RB0
#define ButtonRight PORTBbits.RB1
#define ButtonRotate PORTBbits.RB2
#define ButtonDown PORTBbits.RB3
#define ButtonHold PORTBbits.RB4


void DeviceSetup(void)
{
	//Use Internal Oscillator at 8MHz
	OSCCON = 0x72;
	OSCTUNE = 0x00;
	SSPADD = 0x04;

	//Ports Setup
	LATA = 0x00;
	LATB = 0x00;
	LATC = 0x00;
	LATD = 0x00;
	
	TRISA = 0xFF;
	TRISB = 0xFF;
	TRISC = 0xFF;
	TRISD = 0x3F;
	
	//Timer for 1s (around 1s)
	OpenTimer0(T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
	WriteTimer0(24954);
}

#endif