/*
 * char_display.c
 *
 * Created: 10/22/2019 9:06:12 PM
 *  Author: Bryon
 */ 

#include "char_display.h"

/*
Macros....just in case.
*/
#define PIN(x) (*(&x - 2)) // Address Of Data Direction Register Of Port x
#define DDR(x) (*(&x - 1)) // Address Of Input Register Of Port x
#define PORT(x) (x)


void GPIO_Initialization()
{
	//Set control pins to outputs
	DDR_REGISTER_SELECT |= (1 << PIN_REGISTER_SELECT);
	DDR_READ_WRITE |= (1 << PIN_READ_WRITE);
	DDR_CHIP_ENABLE |= (1 << PIN_CHIP_ENABLE);
	
	//Set the data pins to inputs
	DataPortReadWrite(1);
	
	//Set everything to a default state.
	PORT_REGISTER_SELECT = &= ~(1 << PIN_REGISTER_SELECT);
	PORT_READ_WRITE = &= ~(1 << PIN_READ_WRITE);
	PORT_CHIP_ENABLE = &= ~(1 << PIN_CHIP_ENABLE);
}

/*
Set the data lines to be either read or write.
*/
void DataPortReadWrite(uint8_t direction)
{
	if(direction == 1)
	{
		//Set to input
		DDR_DATA_LOW &= ~((1 << PIN_DB0) | (1 << PIN_DB1) | (1 << PIN_DB2) | (1 << PIN_DB3) | (1 << PIN_DB4) | (1 << PIN_DB5) | (1 << PIN_DB6));
		DDR_DATA_HIGH &= ~(1 << PIN_DB7);			
	}
	else
	{
		//Set to output
		DDR_DATA_LOW |= ((1 << PIN_DB0) | (1 << PIN_DB1) | (1 << PIN_DB2) | (1 << PIN_DB3) | (1 << PIN_DB4) | (1 << PIN_DB5) | (1 << PIN_DB6));
		DDR_DATA_HIGH |= (1 << PIN_DB7);			
	}	
}


/*
Put the data onto the port.
*/
void SetDataToPort(uint8_t nData)
{
	DataPortReadWrite(0);
	
	WRITE_PORT_LOW |= (0x7F & nData);
	WRITE_PORT_HIGH |= ((0x80 & nData) >> 8);
}

/*
Get the data from the port.
*/
uint8_t GetDataFromPort(void)
{
	DataPortReadWrite(1);
	return (READ_PORT_LOW & 0x80) | (READ_PORT_HIGH << 8);
}


/*
Selects register where data is to be sent during
write operations.
H = Data
L = Instruction code
*/
void RegisterSelector(uint8_t val)
{
	if(val)
	{
		PORT_REGISTER_SELECT |= PIN_REGISTER_SELECT;	
	}
	else
	{
		PORT_REGISTER_SELECT &= ~(1<<PIN_REGISTER_SELECT);	
	}
}

/*
Selects whether date is going to be written to the display
or if it will be read from the display.
H = Read
L = Write
*/
void ReadWriteMode(uint8_t val)
{
	if(val)
	{
		PORT_READ_WRITE |= PIN_READ_WRITE;
	}
	else
	{
		PORT_READ_WRITE &= ~(1<<PIN_READ_WRITE);
	}
}

/*
Enables or disables the chip.
H = 
H,H->L = 
*/
void ChipEnable(uint8_t val)
{
	if(val)
	{
		PORT_CHIP_ENABLE |= (1<<PIN_CHIP_ENABLE);
	}
	else
	{
		PORT_CHIP_ENABLE &= ~(1<<PIN_CHIP_ENABLE);
	}
}

/*
Writes data to the device.
data - value to be written to the character display
rs - register to be written to data/command
*/
void WriteData(uint8_t nData, uint8_t rs)
{
	//Check if the busy flag is set. If it is
	//wait till it is done.
	while(1)
	{
		uint8_t reg = 0;
		ReadData(&reg);
		if((reg & 0x80) == 0)
		{
			break;
		}
	}

	RegisterSelector(rs);
	ReadWriteMode(WRITE);
	
	ChipEnable(ENABLE);
	
	SetDataToPort(nData);
	
	ChipEnable(DISABLE);
	
	RegisterSelector(READ);
	ReadWriteMode(READ);	
}

/*
Writes data to the device.
data - address for data to be written to.
*/
void ReadData(uint8_t *data, uint8_t rs)
{
	RegisterSelector(rs);
	ReadWriteMode(READ);
	
	ChipEnable(ENABLE);
	
	(*data) = GetDataFromPort();
	
	ChipEnable(DISABLE);
	
	RegisterSelector(RS_DATA);
	ReadWriteMode(READ);
}

/*
Checks if the busy flag is set. If it is set then it is busy.
Note that the busy flag is the MSB of the data byte.
*/
int CheckBusyFlag()
{
	uint8_t bf;
	ReadData(&bf);
	
	return bf & 0x80;
}

/*
Runs the initialization routine for the 8 bit character display.
*/
void Initialize()
{
	//Function set
	WriteData(CMD_FUNCTION_SET|EIGHTBIT|TWOLINE|FIVE_ELEVEN, RS_INSTRUCTION_CODE);
	
	//Wait at least 39us	
	//Function set
	WriteData(CMD_FUNCTION_SET|EIGHTBIT|TWOLINE|FIVE_ELEVEN, RS_INSTRUCTION_CODE);	
	
	//Wait at least 37us	
	//Display On/Off control
	WriteData(CMD_DISPLAY_CONTROL|DISPLAY_ON|CURSOR_ON|BLINK, RS_INSTRUCTION_CODE);	
	
	//Clear display
	WriteData(CMD_CLEAR_DISPLAY, RS_INSTRUCTION_CODE);
	
	//Set entry mode
	WriteData(CMD_SENTRY_MODE_SET|CURSOR_DIRECTION, RS_INSTRUCTION_CODE);
}

