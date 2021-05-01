#include "simpletetris.h"

static unsigned int gameZone[10];
static unsigned char nextDropZone[2];
static unsigned char holdZone[2];
static unsigned char position[4];
static unsigned char centre;
static unsigned int currentBag[7];
static unsigned int nextBag[7];
static unsigned char isDropping = 0;
static unsigned char gameover = 0;
static unsigned char holdingDisable = 0;
static unsigned char holdingTetris = -1;
static int score = 0;
static int bagIndex = -1;
static int currentIndex = -1;

void StartTetris(void)
{
	unsigned char col;

    for (col = 0; col < 10; col++) {
        gameZone[col] = 0x0000;
    }

	for (col = 0; col < 2; col++) {
        nextDropZone[col] = 0x20;
    }

	//Display Layout Lines
	I2C_Data2(display1, 0x0A, 0xF0, 0xFF);
	I2C_Data2(display1, 0x0E, 0x20, 0x21);
	I2C_Data2(display1, 0x0C, 0x20, 0x21);

	I2C_Data2(display2, 0x04, 0xF0, 0xFF);
	I2C_Data2(display2, 0x02, 0x20, 0x21);
	I2C_Data2(display2, 0x00, 0x20, 0x21);

	I2C_Data2(display3, 0x0A, 0xFF, 0xFF);

	I2C_Data2(display4, 0x04, 0xFF, 0xFF);

	RefreshDisplay();
}
//-----------------------------------------------------------------------------
void WriteTetris(void)
{
	unsigned char green;
	unsigned char row;	
	unsigned char col;
	
	row = 0x08;
	for (col = 0; col < 5; col++) {
		green = gameZone[col] & 0xFF;
		I2C_Data(display1, row, green);
		green = (gameZone[col] >> 8) & 0xFF;
		I2C_Data(display3, row, green);
		row = row - 2;
	}
	row = 0x0E;
	for (col = 5; col < 10; col++) {
		green = gameZone[col] & 0xFF;
		I2C_Data(display2, row, green);
		green = (gameZone[col] >> 8) & 0xFF;
		I2C_Data(display4, row, green);
		row = row - 2;
	}

	RefreshDisplay();
}
//-----------------------------------------------------------------------------
void DropTetris(void)
{
	unsigned char i;

	//Generate the tetris bags
	if (bagIndex == -1) {
		bagIndex = 0;
		SevenBagGenerator(currentBag); 
		SevenBagGenerator(nextBag);
	}

	//When a bag is used up, switch to next bag
	if (bagIndex >= 7) {
		bagIndex = 0;
		for (i = 0; i < 7; i++)
			currentBag[i] = nextBag[i];
		SevenBagGenerator(nextBag);
	}

	//Tetris shapes
	if (isDropping == 0) {
		currentIndex = bagIndex;
		centre = 0x41;			
		switch(currentBag[bagIndex++]) {
		case 1: // O
			position[0] = 0x40;
    		position[1] = 0x50;
    		position[2] = 0x41;
    		position[3] = 0x51;
			break;
		case 2: // I
			position[0] = 0x31;
    		position[1] = 0x41;
    		position[2] = 0x51;
    		position[3] = 0x61;
			break;
		case 3: // S
			position[0] = 0x31;
    		position[1] = 0x40;
    		position[2] = 0x41;
    		position[3] = 0x50;
			break;
		case 4: // Z
			position[0] = 0x30;
    		position[1] = 0x40;
    		position[2] = 0x41;
    		position[3] = 0x51;
			break;
		case 5: // L
			position[0] = 0x30;
    		position[1] = 0x31;
    		position[2] = 0x40;
    		position[3] = 0x50;
			break;
		case 6: // J
			position[0] = 0x30;
    		position[1] = 0x40;
    		position[2] = 0x50;
    		position[3] = 0x51;
			break;
		case 7: // T
			position[0] = 0x30;
    		position[1] = 0x40;
    		position[2] = 0x41;
    		position[3] = 0x50;
			break;
		}

		for (i = 0; i < 4; i++) {
        	setBit(gameZone, position[i]);
    	}

		WriteTetris();
		WriteNextTetris();
		isDropping = 1;
	}    
}
//-----------------------------------------------------------------------------
void MoveTetris(int direction)
{	
	unsigned char i;
	unsigned char stop = 0;

	/*
	* Move directions:
	* 1: Left
	* 2: Right
	* 3: Down
	*/

	if (isDropping == 1) {
		//Move Left
		if (direction == 1) {
			for (i = 0; i < 4; i++) {
            clearBit(gameZone, position[i]);
        	}

			for (i = 0; i < 4; i++) {
            	if ((position[i] & 0xF0) == 0x00 || (testBit(gameZone, position[i] - 0x10) == 1)) {
                	stop = 1;
               	 	break;
            	}
        	}
			if (stop == 0) {
				if ((centre & 0xF0) > 0x10)
					centre = centre - 0x10;
				for (i = 0; i < 4; i++) {
            		position[i] = position[i] - 0x10;
            		setBit(gameZone, position[i]);
        		}
			} else {
				for (i = 0; i < 4; i++) {
        			setBit(gameZone, position[i]);
    			}
			}			
		}

		//Move Right
		if (direction == 2) {
			for (i = 0; i < 4; i++) {
            clearBit(gameZone, position[i]);
        	}

			for (i = 0; i < 4; i++) {
            	if ((position[i] & 0xF0) == 0x90 || (testBit(gameZone, position[i] + 0x10) == 1)) {
                	stop = 1;
               	 	break;
            	}
        	}
			if (stop == 0) {
				if ((centre & 0xF0) < 0x80)
					centre = centre + 0x10;
				for (i = 0; i < 4; i++) {
            		position[i] = position[i] + 0x10;
            		setBit(gameZone, position[i]);
        		}
			} else {
				for (i = 0; i < 4; i++) {
        			setBit(gameZone, position[i]);
    			}
			}			
		}
		
		//Move Down
		if (direction == 3) {
			for (i = 0; i < 4; i++) {
            clearBit(gameZone, position[i]);
        	}	
		
        	for (i = 0; i < 4; i++) {
            	if ((position[i] & 0x0F) == 0x0F || (testBit(gameZone, position[i] + 0x01) == 1)) {
                	isDropping = 0;
                	break;
            	}
        	}

        	if (isDropping == 1) {
				if ((centre & 0x0F) < 0x0E)
					centre = centre + 0x01;
				for (i = 0; i < 4; i++) {
            		position[i] = position[i] + 0x01;
            		setBit(gameZone, position[i]);
        		}
			} else {
				for (i = 0; i < 4; i++) {
        			setBit(gameZone, position[i]);
    			}
			}			
    	}		
		WriteTetris();
	} else if (isDropping == 0){
		TestCompleteRow();
		holdingDisable = 0;		
		if(TestLose() == 0) {
			DropTetris();
		} else {
			GameOver();
			isDropping = 2;
		}
	}   
}
//-----------------------------------------------------------------------------
void RotateTetris(void)
{
	unsigned char i;
	unsigned char j;
	unsigned char onEdge = 0;
	unsigned char outEdge = 0;
	unsigned char type = 0;
	unsigned char isOverlap = 0;
	unsigned char tempPos[4];

	/*
	* onEdge
	* 0: not on edge
	* 4: 'I' piece is on edge
	* 
	* outEdge
	* 0: not out edge
	* 1: out of left edge
	* 2: out of right edge
	* 3: possibly out right edge
	* 4: out of bottom Edge
	* 
	* type
	* 0: 2x2 blocks
	* 1: 3x3 blocks
	* 2: 4x4 blocks
	*/

	if (isDropping == 1 && currentIndex != -1) {		
		switch(currentBag[currentIndex]) {
		case 1: // O
			return;				
		case 2: // I
			type = 2;				
			break;
		case 3: // S
			type = 1;				
			break;
		case 4: // Z
			type = 1;				
			break;
		case 5: // L
			type = 1;			
			break;
		case 6: // J
			type = 1;				
			break;
		case 7: // T
			type = 1;				
			break;
		}

		for (i = 0; i < 4; i++) {
			tempPos[i] = position[i];
			clearBit(gameZone, position[i]);
		}
		
		if (type == 1) {
			// 3x3 default : centre = 5
			for (i = 0; i < 4; i++) {
				if (position[i] == (centre - 0x11)) //1 -> 3
					position[i] = (centre + 0x0F);
				else if (position[i] == (centre - 0x01)) //2 -> 6
					position[i] = (centre + 0x10);
				else if (position[i] == (centre + 0x0F)) //3 -> 9
					position[i] = (centre + 0x11);
				else if (position[i] == (centre - 0x10)) //4 -> 2
					position[i] = (centre - 0x01);
				else if (position[i] == (centre)) //5 -> 5
					position[i] = (centre);
				else if (position[i] == (centre + 0x10)) //6 -> 8
					position[i] = (centre + 0x01);
				else if (position[i] == (centre - 0x0F)) //7 -> 1
					position[i] = (centre - 0x11);
				else if (position[i] == (centre + 0x01)) //8 -> 4
					position[i] = (centre - 0x10);
				else if (position[i] == (centre + 0x11)) //9 -> 7
					position[i] = (centre - 0x0F);				
			}
		} else if (type == 2) {
			//4x4 defaule : centre = 4
			for (i = 0; i < 4; i++) {
				if ((position[i] & 0xF0) == 0x00)
					onEdge++;
				else if ((position[i] & 0xF0) >= 0x80)
					onEdge++;
				else if ((position[i] & 0x0F) >= 0x0E)
					onEdge += 5;
			}
			
			if (onEdge == 4) {
				for (i = 0; i < 4; i++) {
					if ((position[i] & 0xF0) == 0x00) {
						position[i] += 0x10;
						outEdge = 1;
					}
					else if ((position[i] & 0xF0) == 0x90) {
						position[i] -= 0x10;
						outEdge = 2;
					}
					else if (((position[i] & 0xF0) == 0x80) && ((centre & 0xF0) == 0x80)) {
						position[i] -= 0x10;
						outEdge = 3;
					}
				}	
				if (outEdge == 2) centre -= 0x10;
				else if (outEdge == 3) centre -= 0x10;
			} else if (onEdge >= 5) {
				return;
			}

			for (i = 0; i < 4; i++) {
				if (position[i] == (centre - 0x01)) //1 -> 6
					position[i] = (centre + 0x20);
				else if (position[i] == (centre + 0x0F)) //2 -> 10
					position[i] = (centre + 0x21);
				else if (position[i] == (centre - 0x10)) //3 -> 2
					position[i] = (centre + 0x0F);
				else if (position[i] == (centre)) //4 -> 5
					position[i] = (centre + 0x10);
				else if (position[i] == (centre + 0x10)) //5 -> 9
					position[i] = (centre + 0x11);
				else if (position[i] == (centre + 0x20)) //6 -> 12
					position[i] = (centre + 0x12);
				else if (position[i] == (centre - 0x0F)) //7 -> 1
					position[i] = (centre - 0x01);
				else if (position[i] == (centre + 0x01)) //8 -> 4
					position[i] = (centre);
				else if (position[i] == (centre + 0x11)) //9 -> 8
					position[i] = (centre + 0x01);
				else if (position[i] == (centre + 0x21)) //10 -> 11
					position[i] = (centre + 0x02);
				else if (position[i] == (centre + 0x02)) //11 -> 3
					position[i] = (centre - 0x10);
				else if (position[i] == (centre + 0x12)) //12 -> 7
					position[i] = (centre - 0x0F);
			}	
		}
		
		for (i = 0; i < 4; i++) {
			if(testBit(gameZone, position[i]) == 1) {
				isOverlap = 1;
				break;
			}
		}
		
		if (isOverlap == 1) {
			for (i = 0; i < 4; i++) {
				position[i] = tempPos[i];
				setBit(gameZone, position[i]);
			}
		} else {
			for (i = 0; i < 4; i++) {
				setBit(gameZone, position[i]);
			}
		}
		WriteTetris();		
	}	
}
//-----------------------------------------------------------------------------
void HoldTetris(void)
{
	unsigned char temp;	
	unsigned char i;

	if (holdingDisable == 0 && isDropping == 1 && currentIndex != -1) {
		holdingDisable = 1;		
		for (i = 0; i < 4; i++) {
			clearBit(gameZone,position[i]);
		}		
		if (holdingTetris == -1) {
			holdingTetris = currentBag[currentIndex];
			isDropping = 0;
			DropTetris();
		} else {
			temp = currentBag[currentIndex];
			currentBag[currentIndex] = holdingTetris;
			holdingTetris = temp;
			bagIndex--;
			isDropping = 0;
			DropTetris();
		}
		WriteHoldingTetris();
	} else
		return;
}
//-----------------------------------------------------------------------------
void TestCompleteRow(void)
{
	unsigned char x;
	unsigned char y;
	unsigned char i;
	unsigned char isComplete = 0;
	unsigned char highestRow = 15;
	unsigned char lowestRow = 0;
	
	for (i = 0; i < 4; i++) {
		if ((position[i] & 0x0F) < highestRow)
			highestRow = (position[i] & 0x0F);
		if ((position[i] & 0x0F) > lowestRow)
			lowestRow = (position[i] & 0x0F);
	}
	for (y = lowestRow; y >= highestRow; y--) {
		isComplete = 1;
		for (x = 0; x < 10; x++) {
			if (testBit(gameZone, y + 16*x) == 0) {
				isComplete = 0;
				break;
			}
		}
		if (isComplete == 1)
			ClearRow(y++);
	}
}
//-----------------------------------------------------------------------------
unsigned char TestLose(void)
{	
	//Lose if there are blocks at row 3
	unsigned char i;
	for (i = 0; i < 4; i++) {
		if ((position[i] & 0x0F) <= 3)
			return 1;			
	}
	return 0;
}
//-----------------------------------------------------------------------------
void ClearRow(unsigned char row)
{
	unsigned char i;
	unsigned char j;
	isDropping = 0;

	for (i = 0; i < 10; i++) {
		clearBit(gameZone, row + 16*i);
		Delay10KTCYx(5);	//adds visual effect
		WriteTetris();
	}
	for (i = 0; i < 10; i++) {
		for (j = row; j < 15; j++) {
			if (testBit(gameZone, j + 1 + 16*i) == 1) {
				setBit(gameZone, j + 16*i);
			} else {
				clearBit(gameZone, j + 16*i);
			}
		}
	}	
	for (i = 0; i < 10; i++) {
		gameZone[i] = gameZone[i] << 1;
	}
	WriteTetris();
	score += 1;
}
//-----------------------------------------------------------------------------
void SevenBagGenerator(unsigned int bag[7])
{
	unsigned int temp;
	unsigned char i;
	unsigned char randomIndex;

	for (i = 0; i < 7; i++)
		bag[i] = i + 1;
	
	for (i = 0; i < 7; i++) {
		randomIndex = getRemainderof7(ReadTimer0());
		temp = bag[i];
		bag[i] = bag[randomIndex];
		bag[randomIndex] = temp;
	}
}
//-----------------------------------------------------------------------------
void WriteNextTetris()
{
	unsigned char i;	
	unsigned char tetris;
	unsigned char green;
	unsigned char cmd;
	
	for (i = 0; i < 2; i++) {
        nextDropZone[i] = 0b00100000;
    }
	
	if (bagIndex <= 6)
		tetris = currentBag[bagIndex];
	else
		tetris = nextBag[0];

	switch(tetris) {
	case 1: // O
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 3);
		setBit8x(nextDropZone, 10);
		setBit8x(nextDropZone, 11);
		break;
	case 2: // I
		setBit8x(nextDropZone, 1);
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 3);
		setBit8x(nextDropZone, 4);
		break;
	case 3: // S
		setBit8x(nextDropZone, 1);
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 10);
		setBit8x(nextDropZone, 11);
		break;
	case 4: // Z
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 3);
		setBit8x(nextDropZone, 9);
		setBit8x(nextDropZone, 10);
		break;
	case 5: // L
		setBit8x(nextDropZone, 1);
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 3);
		setBit8x(nextDropZone, 11);
		break;
	case 6: // J
		setBit8x(nextDropZone, 3);
		setBit8x(nextDropZone, 9);
		setBit8x(nextDropZone, 10);
		setBit8x(nextDropZone, 11);
		break;
	case 7: // T
		setBit8x(nextDropZone, 2);
		setBit8x(nextDropZone, 9);
		setBit8x(nextDropZone, 10);
		setBit8x(nextDropZone, 11);
		break;
	default: //Error
		WriteNextTetris();
		break;
	} 	

	cmd = 0x02;
	for (i = 0; i < 2; i++) {
		green = nextDropZone[i] & 0xFF;
		I2C_Data(display2, cmd, green);
		cmd = cmd - 2;
	}
	I2C_Command(display2, 0x81);
}
//-----------------------------------------------------------------------------
void WriteHoldingTetris()
{
	unsigned char i;	
	unsigned char green;
	unsigned char cmd;
	
	for (i = 0; i < 2; i++) {
        holdZone[i] = 0b00100000;
    }
	
	switch(holdingTetris) {
	case 1: // O
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 3);
		setBit8x(holdZone, 10);
		setBit8x(holdZone, 11);
		break;
	case 2: // I
		setBit8x(holdZone, 1);
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 3);
		setBit8x(holdZone, 4);
		break;
	case 3: // S
		setBit8x(holdZone, 1);
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 10);
		setBit8x(holdZone, 11);
		break;
	case 4: // Z
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 3);
		setBit8x(holdZone, 9);
		setBit8x(holdZone, 10);
		break;
	case 5: // L
		setBit8x(holdZone, 1);
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 3);
		setBit8x(holdZone, 11);
		break;
	case 6: // J
		setBit8x(holdZone, 3);
		setBit8x(holdZone, 9);
		setBit8x(holdZone, 10);
		setBit8x(holdZone, 11);
		break;
	case 7: // T
		setBit8x(holdZone, 2);
		setBit8x(holdZone, 9);
		setBit8x(holdZone, 10);
		setBit8x(holdZone, 11);
		break;
	} 	

	cmd = 0x0E;
	for (i = 0; i < 2; i++) {
		green = holdZone[i] & 0xFF;
		I2C_Data(display1, cmd, green);
		cmd = cmd - 2;
	}
	I2C_Command(display1, 0x81);
}
//-----------------------------------------------------------------------------
void GameOver(void)
{
	unsigned char i;
	unsigned char j;
	unsigned char red;

	//Clear gameZone
	for (j = 0; j < 16; j++) {
		for (i = 0; i < 10; i++) {
			clearBit(gameZone, j + 16*i);
			Delay10KTCYx(1);
			WriteTetris();
		}
	}

	//Print GameOver
	j = 3;
	for (i = 2; i <= 7; i++)
		setBit(gameZone, j + 16*i);

	j = 4;
	for (i = 1; i <= 2; i++)
		setBit(gameZone, j + 16*i);
	for (i = 7; i <= 8; i++)
		setBit(gameZone, j + 16*i);

	j = 5;
	for (i = 0; i <= 1; i++)
		setBit(gameZone, j + 16*i);
	for (i = 8; i <= 9; i++)
		setBit(gameZone, j + 16*i);

	for (j = 6; j <= 9; j++) {
		i = 0;
		setBit(gameZone, j + 16*i);
		i = 9;
		setBit(gameZone, j + 16*i);
	}
	for (j = 7; j <= 8; j++) {
		i = 1;
		setBit(gameZone, j + 16*i);
		i = 2;
		setBit(gameZone, j + 16*i);
		i = 6;
		setBit(gameZone, j + 16*i);
		i = 7;
		setBit(gameZone, j + 16*i);
	}
	setBit(gameZone, 72);
	
	j = 10;
	for (i = 0; i <= 1; i++)
		setBit(gameZone, j + 16*i);
	for (i = 8; i <= 9; i++)
		setBit(gameZone, j + 16*i);

	setBit(gameZone, 27);
	setBit(gameZone, 59);
	setBit(gameZone, 91);
	setBit(gameZone, 123);
	setBit(gameZone, 139);

	j = 12;
	for (i = 1; i <= 7; i++)
		setBit(gameZone, j + 16*i);
	
	WriteTetris();
	for (j = 0; j <= 10; j++) {
		for (i = 0; i < 10; i++) {
			gameZone[i] = gameZone[i] << 1;
		}
		WriteTetris();
		Delay10KTCYx(300);	
		for (i = 0; i < 10; i++) {
			gameZone[i] = gameZone[i] >> 1;
		}
		WriteTetris();
		Delay10KTCYx(300);
	}

	gameover = 1;
}
//-----------------------------------------------------------------------------
int getScore(void)
{
	return score;
}
//-----------------------------------------------------------------------------
unsigned char isGameover(void)
{
	return gameover;
}
//-----------------------------------------------------------------------------
void reset(void)
{
	isDropping = 0;
	gameover = 0;
	score = 0;
	holdingDisable = 0;
	holdingTetris = -1;
	bagIndex = -1;
	currentIndex = -1;
}