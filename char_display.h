/*
 * char_display.h
 *
 * Created: 10/22/2019 9:07:04 PM
 *  Author: Bryon
 */ 

#include <avr/io.h>

/*

Driver for Character display model no: WH1602B-CFH-JT


Pin No. Symbol Level Description
1 VSS 0V Ground
2 VDD 5.0V Supply Voltage for logic
3 VO (Variable) Operating voltage for LCD
4 RS H/L H: DATA, L: Instruction code
5 R/W H/L H: Read(MPU?Module) L: Write(MPU?Module)
6 E H,H?L Chip enable signal
7 DB0 H/L Data bus line
8 DB1 H/L Data bus line
9 DB2 H/L Data bus line
10 DB3 H/L Data bus line
11 DB4 H/L Data bus line
12 DB5 H/L Data bus line
13 DB6 H/L Data bus line
14 DB7 H/L Data bus line
15 A - Supply power for LED +
16 R - Supply power for Red -
17 G Supply power for Green -
18 B Supply power for Blue -
*/

#define RS_DATA 				0x1
#define RS_INSTRUCTION_CODE 	0x0
#define READ					0x1
#define WRITE					0x0
#define ENABLE					0x1
#define	DISABLE					0x0

/*
*	Definitions for the hardware interface.
*/
#define PIN_REGISTER_SELECT		PIND1
#define PIN_READ_WRITE			PIND2
#define PIN_CHIP_ENABLE			PIND3

#define PIN_DB0					PINB0
#define PIN_DB1					PINB1
#define PIN_DB2					PINB2
#define PIN_DB3					PINB3
#define PIN_DB4					PINB4
#define PIN_DB5					PINB5
#define PIN_DB6					PINB6
#define PIN_DB7					PIND0

#define PORT_REGISTER_SELECT	PORTD
#define PORT_CHIP_ENABLE		PORTD
#define PORT_READ_WRITE			PORTD

#define DDR_REGISTER_SELECT		DDRD
#define DDR_CHIP_ENABLE			DDRD
#define DDR_READ_WRITE			DDRD
#define DDR_DATA_LOW			DDRB
#define DDR_DATA_HIGH			DDRD

#define READ_PORT_LOW			PINB
#define READ_PORT_HIGH			PIND

#define WRITE_PORT_LOW			PORTB
#define WRITE_PORT_HIGH			PORTD


/*
Write “00H” to DDRAM and set
DDRAM address to “00H” from AC
0 0 0 0 0 0 0 0 0 1
*/
#define CMD_CLEAR_DISPLAY 0x01		//RS = 0 R/W = 0

/*
Set DDRAM address to “00H” from AC
and return cursor to its original position
if shifted. The contents of DDRAM are
not changed.
0 0 0 0 0 0 0 0 1 -
*/
#define CMD_RETURN_HOME 0x02			//RS = 0 R/W = 0

/*
Assign cursor moving direction and enable the shift of entire display.
0 0 0 0 0 0 0 1 I/D SH
*/
#define CMD_SENTRY_MODE_SET 0x04		//RS = 0 R/W = 0
#define CURSOR_DIRECTION	0x2
#define SHIFT_DISPLAY		0x1

/*
Set display (D), cursor (C), and blinking of cursor (B) on/off control bit.
0 0 0 0 0 0 1 D C B
*/
#define CMD_DISPLAY_CONTROL 0x08		//RS = 0 R/W = 0
#define DISPLAY_ON			0x04
#define CURSOR_ON			0x02
#define BLINK				0x01

/*
Set cursor moving and display shift
control bit, and the direction, without
changing of DDRAM data.
0 0 0 0 0 1 S/C R/L - -
*/
#define CMD_CURSOR_SHIFT 	0x10		//RS = 0 R/W = 0
#define CURSOR_ROIGHT		0x04
#define SHIFT_CONTROL		0x08

/*
Set interface data length
(DL:8-bit/4-bit), numbers of display line
(N:2-line/1-line)and, display font type
(F:5×11 dots/5×8 dots)
0 0 0 0 1 DL N F - -
*/
#define CMD_FUNCTION_SET 	0x20		//RS = 0 R/W = 0
#define EIGHTBIT			0x10
#define TWOLINE			0x08
#define FIVE_ELEVEN		0x04

/*
Set CGRAM address in address counter.
0 0 0 1 AC5 AC4 AC3 AC2 AC1 AC0
*/
#define CMD_SETCGRAM_ADDR 0x40		//RS = 0 R/W = 0

/*
Set DDRAM address in address counter.
0 0 1 AC6 AC5 AC4 AC3 AC2 AC1 AC0

Character Located		1		16
Upper row 				0x00 - 0x0F
Lower row 				0x40 - 0x4F
*/
#define CMD_SETDDRAM_ADDR 0x80		//RS = 0 R/W = 0

/*
Set DDRAM address in address counter.
0 1 BF AC6 AC5 AC4 AC3 AC2 AC1 AC0
*/
#define CMD_READ_BF 		0x80		//RS = 0 R/W = 1

/*
Write data into internal RAM
(DDRAM/CGRAM)
1 0 D7 D6 D5 D4 D3 D2 D1 D0
*/
#define CMD_WRITE_DATA		0x100		//RS = 1 R/W = 0

/*
Write data into internal RAM
(DDRAM/CGRAM)
1 0 D7 D6 D5 D4 D3 D2 D1 D0
*/
#define CMD_READ_DATA		0x200		//RS = 1 R/W = 1

/*
Functions.
*/
void GPIO_Initialization(void);
void Initialize(void);
void ReadData(uint8_t *, uint8_t);
void WriteData(uint8_t *, uint8_t);
#ifndef CHAR_DISPLAY_H_
#define CHAR_DISPLAY_H_
#endif /* CHAR_DISPLAY_H_ */