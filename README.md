# PIC18F-Tetris
Tetris game which runs on a PIC18

## Hardware
- PIC18F4520
- HT16K33
- 8x8 Bi-Colour Common-Cathode LED Matrix
- 13mm Seven Segment Display, Common-Cathode
- 74HC14
- Some cherrymx compatible switches

## Program
- Buttons polling used to read the buttons
- simpletetris.c is the main game program, also directly write to the displays
- Device configuration code in devcon.h

## Tetris
Has basic Tetris features including moving left, right, down and rotating the piece.
Also for holding one Tetris piece in top-left corner.

## Compiling
The program could be compiled with the legacy MPLAB C18 C Complier. 

## Pictures
![pcb-in-enclosure](https://github.com/Hyphen-and-Hyphens/PIC18F-Tetris/blob/main/Pictures/With%20Enclosure.jpg)
