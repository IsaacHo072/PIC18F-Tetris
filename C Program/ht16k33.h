#ifndef HT16K33
#define HT16K33

#include "helper.h"
#include <delays.h>

//Turn On the 5 Displays
void InitializeDisplay(void);

//Display a test pattern
void TestDisplay(void);

//Clear All Displays
void ClearDisplay(void);

//Refresh All Displays
void RefreshDisplay(void);

//Write a number to the 7-segment Display
void WriteNumber(int number);

#endif