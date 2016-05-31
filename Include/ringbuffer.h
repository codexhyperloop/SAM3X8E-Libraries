/*
 * ringbuffer.h
 *
 * Created: 12/18/2015 5:26:01 PM
 *  Author: Brandon
 */ 


#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include "../libraries.h"

#define BUFFER_SIZE 128

class RingBufferClass{
	//Variables
	public:
	volatile uint8_t ringBuffer[BUFFER_SIZE];
	volatile uint32_t readLocation;
	volatile uint32_t writeLocation;
	
	//Functions
	public:
		RingBufferClass(void);
		void	eraseBuffer();
		void	storeData(uint8_t data);
		uint8_t readData();
		void	storeData(uint8_t dataBuffer[], uint32_t numBytes); //Untested
		void	readData(uint8_t readBuffer[], uint32_t numBytes);	//Untested
		uint8_t memAvailable();
		uint8_t memUsed();
		void	freeSpace(uint32_t numBytes);
};

#endif /* RINGBUFFER_H_ */