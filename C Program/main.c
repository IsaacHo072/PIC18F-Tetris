#include <p18F4520.h>

#include "devcon.h"

static unsigned char buttonDisable = 0;

void main(void){

	DeviceSetup();
	Delay1KTCYx(4);	//wait 1ms for display controllers to turn on
	OpenI2C(MASTER,SLEW_OFF);
	InitializeDisplay(); 
	ClearDisplay();
	StartTetris();
	Delay10KTCYx(10);
	DropTetris();

	while(1) {	
		while (isGameover() == 0) {
			if (ButtonLeft == 1 && buttonDisable == 0){
				MoveTetris(1);
				LED2 = 1;
				buttonDisable = 1;
			} else if (ButtonRight == 1 && buttonDisable == 0){
				MoveTetris(2);
				LED2 = 1;
				buttonDisable = 1;			
			} else if (ButtonRotate == 1 && buttonDisable == 0){	
				RotateTetris();
				LED2 = 1;
				buttonDisable = 1;
			} else if (ButtonDown == 1){
				MoveTetris(3);
				Delay10KTCYx(10);
			} else if (ButtonHold == 1){
				HoldTetris();
				Delay10KTCYx(10);
			}
			
			if (ButtonLeft == 0 && ButtonRight == 0 && ButtonRotate == 0) {
				LED2 = 0;
				buttonDisable = 0;
			}

			if (ReadTimer0() >= 32767){
				OpenTimer0(T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
				WriteTimer0(24954);
				MoveTetris(3);
				WriteNumber(getScore());
			}
		}

		while (isGameover() == 1) {
			if (ButtonHold == 1){
				reset();
				ClearDisplay();		
				StartTetris();
				Delay10KTCYx(100);
				DropTetris();
			}
		}	
	}
}

