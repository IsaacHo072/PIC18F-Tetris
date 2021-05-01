# PIC18F-Tetris
Tetris game which runs on a PIC18

## Hardware
- PIC18F4520
- HT16K33
- 8x8 Bi-Colour Common-Cathode LED Matrix
- 13mm Seven Segment Display, Common-Cathode
- 74HC14
- Some cherry-mx compatible switches

## Program
- Buttons polling used to read the buttons
- simpletetris.c is the main game program, also directly write to the displays
- Device configuration code in devcon.h

## Tetris
Has basic Tetris features including moving left, right, down and rotating the piece.
Also for holding one Tetris piece in top-left corner. Top-right corner shows the next piece to be dropped.

## Compiling
The program could be compiled with the legacy MPLAB C18 C Complier. 
Change the device configuration and I2C function to I2C1/2 if a different PIC18 microcontroller is used.

## Pictures
<img src="https://github.com/isaacho72/PIC18F-Tetris/blob/main/Pictures/With%20Enclosure.jpg" height="500" />
