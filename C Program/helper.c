#include "helper.h"

// Mathematical functions

int getDivision(int dividend, int divisor)
{
	int result = 0;
	while (dividend >= divisor) {
		dividend = dividend - divisor;
		result++;
	}
	return result;
}

int getRemainder(int dividend, int divisor)
{
	while (dividend >= divisor) {
		dividend = dividend - divisor;
	}
	return dividend;
}

//-----------------------------------------------------------------------------

int getRemainderof7(unsigned int number)
{
	while (number >= 7){
		number = number - 7;
	}
	return number;
}

int getRemainderof8(int number)
{
	return getRemainder(number, 8);
}

int getRemainderof10(int number)
{
	return getRemainder(number, 10);
}

int getRemainderof16(int number)
{
	return getRemainder(number, 16);
}

//-----------------------------------------------------------------------------

int getDivisionof8(int number)
{
	return getDivision(number, 8);
}

int getDivisionof10(int number)
{
	return getDivision(number, 10);
}

int getDivisionof16(int number)
{
	return getDivision(number, 16);
}

int getDivisionof100(int number)
{
	return getDivision(number, 100);
}

//-----------------------------------------------------------------------------
// I2C Functions

void I2C_Command(unsigned char address, unsigned char cmd)
{
	StartI2C();
	//Command
	WriteI2C(address);
	WriteI2C(cmd);
	StopI2C();
}

void I2C_Data(unsigned char address, unsigned char cmd, unsigned char data)
{
	StartI2C();
	//Command and Data
	WriteI2C(address);
	WriteI2C(cmd);
	WriteI2C(data);
	StopI2C();
}

void I2C_Data2(unsigned char address, unsigned char cmd, unsigned char data1, unsigned char data2)
{
	StartI2C();
	//Command and 2 Data
	WriteI2C(address);
	WriteI2C(cmd);
	WriteI2C(data1);
	WriteI2C(data2);
	StopI2C();
}

//-----------------------------------------------------------------------------
// Bit Array Operations

void setBit(unsigned int* A, int k)
{
	int i = getDivisionof16(k);			// i = array index (use: A[i])
	int pos = getRemainderof16(k);		// pos = bit position in A[i]
	unsigned int flag = 1;				// flag = 0000.....00001
	flag = flag << pos;					// flag = 0000...010...000   (shifted k positions)
	A[i] = A[i] | flag;					// Set the bit at the k-th position in A[i]
}

void setBit8x(unsigned char* A, int k)
{
	int i = getDivisionof8(k);			// i = array index (use: A[i])
	int pos = getRemainderof8(k);		// pos = bit position in A[i]
	unsigned int flag = 1;				// flag = 0000.....00001
	flag = flag << pos;					// flag = 0000...010...000   (shifted k positions)
	A[i] = A[i] | flag;					// Set the bit at the k-th position in A[i]
}

void clearBit(unsigned int* A, int k)
{
	int i = getDivisionof16(k);			// i = array index (use: A[i])
	int pos = getRemainderof16(k);		// pos = bit position in A[i]
	unsigned int flag = 1;				// flag = 0000.....00001
	flag = flag << pos;					// flag = 0000...010...000   (shifted k positions)
	flag = ~flag;
	A[i] = A[i] & flag;					// Set the bit at the k-th position in A[i]
}

unsigned char testBit(unsigned int* A, int k)
{
	int i = getDivisionof16(k);			// i = array index (use: A[i])
	int pos = getRemainderof16(k);
	unsigned int flag = 1;				// flag = 0000.....00001
	flag = flag << pos;					// flag = 0000...010...000   (shifted k positions)
	if (A[i] & flag)					// Test the bit at the k-th position in A[i]
		return 1;
	else
		return 0;
}