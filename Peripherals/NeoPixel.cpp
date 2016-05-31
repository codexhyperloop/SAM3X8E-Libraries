/* 
* NeoPixel.cpp
*
* Created: 5/27/2016 4:03:18 PM
* Author: Brandon
*/


#include "../libraries.h"


/* Test only*/
void NeoPixel::test(){
	writeByte(0x00);
	
	latchData();
}

void NeoPixel::initialize(){
	//Setup the pin
	pinMode(_pin, OUTPUT);
	clearPin();
	latchData();
}

void NeoPixel::writePixel(uint8_t red, uint8_t green, uint8_t blue){
	writeByte(green);
	writeByte(red);
	writeByte(blue);
}

void NeoPixel::pushFrame(uint8_t **colorArray){
	
}

// default constructor
NeoPixel::NeoPixel(uint8_t numPixels, uint8_t pin, int refreshRate){
	_nPixels = numPixels;
	_refreshRate = refreshRate;
	_pin = pin;
	_pinMask = lookUp_Mask(_pin);
	_pinPort = lookUp_Port(_pin);
	

	//These are divided by two because for/while loops take a 
	//clock cycle to increment a variable. 
	clockCycles_0H = (TIME_0H*clockPeriod)/2;
	clockCycles_1H = (TIME_1H*clockPeriod)/2;
	clockCycles_LOW = (TIMELOW*clockPeriod)/2;
	clockCycles_LATCH = (T_LATCH*clockPeriod)/2;
	
	//Initialize the data array to zero
	for(uint8_t x=0; x<MAXFRAMES; x++){
		for(uint8_t y=0; y<numPixels; y++){
			for(uint8_t z=0; z<3; z++){
				dataPTR[x][y][z] = 0;
			}
		}
	}
} //NeoPixel


/************************************************************************/
/* Private Functions                                                    */
/************************************************************************/

/// Bare-metal function to set data pin...slow
void NeoPixel::setPin(){
	_pinPort->PIO_SODR |= _pinMask;
}

/// Bare-metal function to clear data pin...slow
void NeoPixel::clearPin(){
	_pinPort->PIO_CODR |= _pinMask;
}

/// Basic function to write valid 1 bit to NeoPixel
void NeoPixel::write1(){
	//Set the data pin high to initialize bit
	_pinPort->PIO_SODR |= _pinMask;

	//Delays about 600nS
	for(int x=0; x<11; x++){
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
	}

	//Clear the data pin
	_pinPort->PIO_CODR |= _pinMask;

	//Wait a number of clock cycles in the low state....
	for(int x=0; x<12; x++){
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
	}
	
}

/// Basic function to write valid 0 bit to NeoPixel
void NeoPixel::write0(){
	//Set the data pin high to initialize bit
	_pinPort->PIO_SODR |= _pinMask;

	//Wait a number of clock cycles in the high state....
	for(int x=0; x<10; x++){
		asm volatile("NOP");
		asm volatile("NOP");
	}
	
	//Clear the data pin
	_pinPort->PIO_CODR |= _pinMask;

	//Wait a number of clock cycles in the low state....
	for(int x=0; x<15; x++){
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
	}
}

/// Basic function to latch the written data to NeoPixel
volatile void NeoPixel::latchData(){
	//The data pin should already be low, so this is 
	//essentially just a 7000 nS delay function.
	clearPin();
	for(int x=0; x<60; x++){
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
	}
}

volatile void NeoPixel::delay350(){
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
}

volatile void NeoPixel::delay700(){
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	
	
}

/// Basic function to write a single byte on the data line
volatile void NeoPixel::writeByte(uint8_t data){
// 	for(uint8_t index=8; index>0; index--){
// 		if(data & (1<<(index-1)))
// 		write1();
// 		else
// 		write0();
// 	}
	//Write highest bit first
	if(data & 1<<7){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
	} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	} 
	_pinPort->PIO_CODR |= _pinMask;
	delay700();


	if(data & 1<<6){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();

	if(data & 1<<5){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();


	if(data & 1<<4){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();

	if(data & 1<<3){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();


	if(data & 1<<2){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();

	if(data & 1<<1){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();


	if(data & 1<<0){
		_pinPort->PIO_SODR |= _pinMask;
		delay700();
		} else {
		_pinPort->PIO_SODR |= _pinMask;
		delay350();
	}
	_pinPort->PIO_CODR |= _pinMask;
	delay700();
}