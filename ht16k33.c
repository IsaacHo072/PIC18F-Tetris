#include "ht16k33.h"

void InitializeDisplay(void)
{
	/*
	* Address of displays:
	* 0xE0, 0xE2, 0xE4, 0xE8, 0xEC
	*/
	unsigned char address = 0xE0;
	
	/*
	* Display Initialisation
	* 1. Power On and Internal system clock enable (0x21)
	* 2. ROW/INT output pin set (0xA0)
	* 3. Dimming Set (0xE0-0xEF)
	* 4. Blinking Set and Display ON (0x80-0x87)
	*/

	for (address; address <= 0xEC; address=address+2){
		if ((address == 0xE6) || (address == 0xEA)){
			continue;
		}
		I2C_Command(address, 0x21);
		I2C_Command(address, 0xA0);
		I2C_Command(address, 0xE2);
		I2C_Command(address, 0x81);
	}
}

void TestDisplay(void)
{
	unsigned char address = 0xE0;
	unsigned char cmd;
	unsigned char data;
	unsigned char data1;
	
	//7-segment Display
	I2C_Data(0xE0, 0x06, 0x06);	//1
	I2C_Data(0xE0, 0x04, 0x5B);	//2
	I2C_Data(0xE0, 0x02, 0x4F);	//3
	I2C_Data(0xE0, 0x00, 0x66);	//4
	I2C_Command(0xE0, 0x81);	//Refresh
	
	//LED Matrix Display
	for (address=0xE2; address <= 0xEC; address=address+2){
		if ((address == 0xE6) || (address == 0xEA)){
			continue;
		}
		//Write a checkerboard pattern
		for (cmd = 0x00; cmd<=0x06; cmd=cmd+2){
			data = 0xFF;	//green	msb
			data1 = 0xF0;	//red	lsb
			I2C_Data2(address, cmd, data, data1);
		}
		for (cmd = 0x08; cmd<=0x0E; cmd=cmd+2){
			data = 0xF0;	//green	msb
			data1 = 0x0F;	//red	lsb
			I2C_Data2(address, cmd, data, data1);
		}
		//Refresh
		I2C_Command(address, 0x81);
	}
}

void ClearDisplay(void)
{
	//Clear the RAM of the display to 0x00
	unsigned char address = 0xE0;
	unsigned char cmd = 0x00;
	
	for (address; address <= 0xEC; address=address+2){
		if ((address == 0xE6) || (address == 0xEA)){
			continue;
		}
		for (cmd = 0x00; cmd <= 0x0F ; cmd++){
			//Set Zeros
			I2C_Data(address, cmd, 0x00);
		}
	}
}

void RefreshDisplay(void)
{
	//Refresh All Display
	unsigned char address = 0xE0;
	unsigned char cmd = 0x81;

	for (address; address <= 0xEC; address = address + 2) {
		if ((address == 0xE6) || (address == 0xEA)) {
			continue;
		}
		I2C_Command(address, cmd);
	}
}

void WriteNumber(int number)
{
	//Writes a number on the 7-segment display
	unsigned char data;
	unsigned char cmd = 0x00;
	int getnumber;
	unsigned char i = 0;
	for (i; i < 4; i++){
		getnumber = getRemainderof10(number);
		switch (getnumber){
		case 0:
			if (i == 0){
				data = 0x3F;
			}
			else if(i == 1 && number >= 10){
				data = 0x3F;
			}
			else if(i == 2 && number >= 10){
				data = 0x3F;
			}
			else{
				data = 0x00;
			}
            break;
		case 1:
			data = 0x06;
            break;
		case 2:
			data = 0x5B;
            break;
		case 3:
			data = 0x4F;
            break;
		case 4:
			data = 0x66;
            break;
		case 5:
			data = 0x6D;
            break;
		case 6:
			data = 0x7D;
            break;
		case 7:
			data = 0x07;
            break;
		case 8:
			data = 0x7F;
            break;
		case 9:
			data = 0x6F;
            break;
		default:
			data = 0x00;
            break;
		}
		number = getDivisionof10(number);
		//Write Out
		I2C_Data(0xE0, cmd, data);
		cmd = cmd + 2;
	}
	//Refresh Display
	I2C_Command(0xE0, 0x81);
}