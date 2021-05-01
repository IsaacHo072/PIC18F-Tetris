#ifndef HELPER
#define HELPER

#include <i2c.h>

/*
* Mathematical Functions
* calculate the remainder and division of a number
*/
int getRemainder(int dividend, int divisor);
int getRemainderof7(unsigned int number);
int getRemainderof8(int number);
int getRemainderof10(int number);
int getRemainderof16(int number);

int getDivision(int dividend, int divisor);
int getDivisionof8(int number);
int getDivisionof10(int number);
int getDivisionof16(int number);
int getDivisionof100(int number);

/*
* I2C Functions
* for sending:
* 1. 1 Command byte
* 2. 1 Command and 1 data byte
* 3. 1 Command and 2 data byte
*/
void I2C_Command(unsigned char address, unsigned char cmd);
void I2C_Data(unsigned char address, unsigned char cmd, unsigned char data);
void I2C_Data2(unsigned char address, unsigned char cmd, unsigned char data1, unsigned char data2);

/*
* Bit Array Operations
* for setting/clearing/testing bits in different types
* of container e.g. char, int
* 
* A is the pointer to the array
* k is the kth bit in the array
*/

void setBit(unsigned int* A, int k);
void setBit8x(unsigned char* A, int k);
void clearBit(unsigned int* A, int k);
unsigned char testBit(unsigned int* A, int k);


#endif