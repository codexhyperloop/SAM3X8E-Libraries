/*
 * ringbuffer.cpp
 *
 * Created: 12/18/2015 5:29:18 PM
 *  Author: Brandon
 */ 

#include "../libraries.h"
//#include <string.h>
//#include <stdlib.h>

//Constructor
RingBufferClass::RingBufferClass(void){
	//Initialize everything to zero
	writeLocation = 0;
	readLocation = 0;
	memset((void *)ringBuffer, 0, BUFFER_SIZE);
}

//Destroys all data and sets buffer to zero
void RingBufferClass::eraseBuffer(){
	memset((void *)ringBuffer, 0, BUFFER_SIZE);	
	readLocation = 0;
	writeLocation = 0;
}

//Stores a single byte to the ringBuffer
void RingBufferClass::storeData(uint8_t data){
	uint32_t nextLocation = (uint32_t)(writeLocation + 1) % BUFFER_SIZE;
	
	//Writes byte to the buffer at specified writeLocation. If the buffer is 
	//full (nextlocation == readlocation), then skip writing and the data is lost.
	if(nextLocation != readLocation){
		ringBuffer[writeLocation] = data;
		writeLocation = nextLocation;
	}
}

//Reads a single byte from the ringBuffer
uint8_t RingBufferClass::readData(){
	uint32_t nextLocation = (uint32_t)(readLocation + 1) % BUFFER_SIZE;
	uint8_t data;
	
	//Reads byte from the buffer at specified readLocation. If the buffer is
	//full (nextlocation == writelocation), then skip reading and the data is lost.
	if(nextLocation != writeLocation){
		data = ringBuffer[readLocation];
		readLocation = nextLocation;
		return data; //Read Success
	}
	return 0; //Read Fail: Buffer full
}

//Writes a number of bytes to the ringBuffer from an external array
void RingBufferClass::storeData(uint8_t writeArray[], uint32_t numBytes){
	uint32_t nextLocation = (uint32_t)(writeLocation + 1) % BUFFER_SIZE;
	
	for(uint32_t index=0; index<numBytes; ++index){
		ringBuffer[writeLocation] = writeArray[index];
		
		//Make sure buffer is not full
		if(nextLocation != readLocation){
			writeLocation = nextLocation;
			nextLocation = (uint32_t)(writeLocation + 1) % BUFFER_SIZE;
		}
	}
}

//Reads a number of bytes from the ringBuffer into an external array.
void RingBufferClass::readData(uint8_t readArray[], uint32_t numBytes){
	uint32_t nextLocation = (uint32_t)(readLocation + 1) % BUFFER_SIZE;
	
	for(uint32_t index=0; index<numBytes; ++index){
		readArray[index] = ringBuffer[readLocation];
		
		//Make sure you aren't going to run into the writeLocation.
		if(nextLocation != writeLocation){
			readLocation = nextLocation;
			nextLocation = (uint32_t)(readLocation + 1) % BUFFER_SIZE;	
		}
	}
}

//Returns how much memory is available to be written to the buffer
uint8_t RingBufferClass::memAvailable(){
	return (uint8_t)(BUFFER_SIZE - memUsed());
}

//Returns how much memory has already been written to the buffer
uint8_t RingBufferClass::memUsed(){
	return (uint8_t)(BUFFER_SIZE + abs(writeLocation - readLocation)) % BUFFER_SIZE;
}

//Frees memory up to capacity of the buffer
void RingBufferClass::freeSpace(uint32_t numBytes){
	uint32_t maxDisplacement = memUsed();
	
	if(numBytes <= maxDisplacement)
		writeLocation -= numBytes;
}