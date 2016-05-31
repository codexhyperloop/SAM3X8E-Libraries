/* 
* NeoPixel.h
*
* Created: 5/27/2016 4:03:19 PM
* Author: Brandon
*/


#ifndef __NEOPIXEL_H__
#define __NEOPIXEL_H__

#include "../libraries.h"

/* Timing for high and low pulses (nS)*/
#define TIME_0H	200		//0-bit time high
#define TIME_1H	550		//1-bit time high
#define TIMELOW	450		//0 & 1 bit time low
#define T_LATCH 6000	//Data latch time low
#define clockPeriod	13	

/* Data array definitions*/
#define MAXFRAMES 5 //Max number of frames to store
#define COLOR_RES 8 //Color resolution in bits 

class NeoPixel
{
//variables
public:


protected:
private:

	int _nPixels;			//Stores number of pixels
	int _refreshRate;		//Refresh rate in Hz
	uint32_t _pin;			//Stores data pin number
	uint32_t _pinMask;
	Pio *_pinPort;

	uint8_t frameData[MAXFRAMES][15][3];	//Stores pixel frames [frame#][pixel#][colorData]
	uint8_t ***dataPTR;

//Variables to store number of NOPs to run
	int clockCycles_0H,
		clockCycles_1H,
		clockCycles_LOW,
		clockCycles_LATCH;

//functions
public:
	void initialize();
	void writePixel(uint8_t red, uint8_t green, uint8_t blue);
	void pushFrame(uint8_t **colorArray);
	NeoPixel(uint8_t numPixels, uint8_t pin, int refreshRate);

	/* Test only*/
	void test();
	

private:
	void setPin();
	void clearPin();
	void write1();
	void write0();
	volatile void latchData();
	volatile void delay350();
	volatile void delay700();
	volatile void writeByte(uint8_t data);
	

}; //NeoPixel

#endif //__NEOPIXEL_H__
