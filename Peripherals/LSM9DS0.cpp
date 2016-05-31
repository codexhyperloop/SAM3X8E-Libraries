/* 
* LSM9DS0.cpp
*
* Created: 2/21/2016 10:46:37 AM
* Author: Brandon
*/


#include "../libraries.h"

/** Default Constructor*/
LSM9DS0::LSM9DS0(int32_t sensorID){
	initI2C(&Wire, sensorID);
}

/** Secondary Constructor*/
LSM9DS0::LSM9DS0(TwoWireClass *wireBus, int32_t sensorID){
	initI2C(wireBus, sensorID);
}


void LSM9DS0::initI2C(TwoWireClass *wireBus, int32_t sensorID){
	_wireBus = wireBus;
	sensorID_Accel	= sensorID + 1;
	sensorID_Mag	= sensorID + 2;
	sensorID_Gyro	= sensorID + 3;
	sensorID_Temp	= sensorID + 4;
}


/************************************************************************/
/* Public Functions                                                     */
/************************************************************************/
bool LSM9DS0::begin(){
	//Initialize I2C interface
	_wireBus->begin();
	
	
	
	//NOT FINISHED WITH THIS FUNCTION
	
	
	
	return true;
}


/************************************************************************/
/* Private Functions                                                    */
/************************************************************************/
void LSM9DS0::write8(bool type, uint8_t reg, uint8_t value){
	uint8_t address;
	
	//Saves the slave address for either the accel/mag or gyro
	if(type == GYROTYPE)
		address = LSM9DS0_ADDRESS_GYRO;
	else
		address = LSM9DS0_ADDRESS_ACCELMAG;
	
	_wireBus->beginTransmission(address);
	_wireBus->write(reg);
	_wireBus->write(value);
	_wireBus->endTransmission();
}

uint8_t LSM9DS0::read8(bool type, uint8_t reg){
	uint8_t value=0;
	
	//readIntoBuffer(type, reg, 1);
	
	return value;
}

uint8_t LSM9DS0::readIntoBuffer(bool type, uint8_t reg, uint8_t len){
	uint8_t address;
	
	//Saves the slave address for either the accel/mag or gyro
	if(type == GYROTYPE)
		address = LSM9DS0_ADDRESS_GYRO;
	else
		address = LSM9DS0_ADDRESS_ACCELMAG;
	
	//Tell the slave we want to read from it
	_wireBus->beginTransmission(address);
	_wireBus->write(reg);
	_wireBus->endTransmission();
	
	//Capture "len" number of bytes from slave into I2C rxBuffer
	_wireBus->requestFrom(address, len);
	
	//Wait until all the data is available
	while(_wireBus->availableToRead() < len);
	
	//Transfer data between buffers
	for(uint8_t x=0; x<len; x++)
		rxBuffer.storeData(_wireBus->read());
	
	_wireBus->endTransmission();
	
	return len;
}