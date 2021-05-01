#ifndef SIMPLE_TETRIS
#define SIMPLE_TETRIS

#include "helper.h"
#include "ht16k33.h"
#include <delays.h>
#include <i2c.h>

#define display1 0xE8
#define display2 0xE4
#define display3 0xEC
#define display4 0xE2

//Initialize the game 
void StartTetris(void);

//Drop a tetris from the bag
void DropTetris(void);

//Move the tetris block
void MoveTetris(int);

//Rotate the tetris block
void RotateTetris(void);

//Move current block into hold
void HoldTetris(void);

//returns score
int getScore(void);

//returns gameover
unsigned char isGameover(void);

//resets game
void reset(void);

//Write game to display
void WriteTetris(void);

//Test matrix row completion
void TestCompleteRow(void);

//Test if lose
unsigned char TestLose(void);

//Clear a row
void ClearRow(unsigned char row);

//Generate tetris blocks random bags
void SevenBagGenerator(unsigned int bag[7]);

//Write the next tetris block to the top right corner
void WriteNextTetris(void);

//Write the holded tetris block to the top left corner
void WriteHoldingTetris(void);

//Gameover display
void GameOver(void);

#endif