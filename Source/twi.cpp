/*
 * twi.cpp
 *
 * Created: 11/23/2015 8:16:23 PM
 *  Author: Brandon
 */ 

/*
Design Notes:
	1) Use beginTransmission to set master into send mode. Any writes will be buffered up to 128 bytes
	   in the txBuffer array. Ending the transmission triggers a write of all data to the slave.
*/


#include "../libraries.h"



/************************************************************************/
/* Class Definition                                                     */
/************************************************************************/

/** Constructor*/
TwoWireClass::TwoWireClass(Twi *pTwi){
	twi = pTwi;
	twiClock = 400000;
	txBufferLength = 0;
	rxBufferLength = 0;
	svBufferLength = 0; 
	rxBufferIndex = 0;
	svBufferIndex = 0;
	
	//Save pin numbers
	if(twi == TWI1){ //Wire
		SDA = 20;
		SCL = 21;
		periphID = ID_TWI1;
		IRQnID = TWI1_IRQn;
	} else {		 //Wire1
		SDA = 70;
		SCL = 71;
		periphID = ID_TWI0;
		IRQnID = TWI0_IRQn;
	}
	
	status = UNINITIALIZED;
}

/** Initializes as I2C Master*/
void TwoWireClass::begin(){
	setupHardware();
	
	TWI_ConfigureMaster(twi, twiClock, MASTER_CLOCK);
	status = MASTER_IDLE;
}

/** Initializes as I2C Slave with slave address*/
void TwoWireClass::begin(uint8_t address){
	setupHardware();
	
	TWI_ConfigureSlave(twi, address);
	TWI_EnableIt(twi, TWI_IER_SVACC);

	status = SLAVE_IDLE;
}

/** Prepares software for transmitting*/
void TwoWireClass::beginTransmission(uint8_t address){
	status = MASTER_SEND;
	
	//Save slave address and empty the transmit buffer
	txAddress = address;
	txBufferLength = 0;
}

/** Empties the transmit buffer by writing to slave device*/
uint8_t TwoWireClass::endTransmission(){
	uint8_t error = 0;
	
	//Send the first byte and wait for ACK	
	TWI_StartWrite(twi, txAddress, 0, 0, txBuffer[0]);
	
	if(!TWI_WaitByteSent(twi, XMIT_TIMEOUT))
		error = 2;	// error, got NACK on address transmit
		
	//Send the entire buffer with ACK after each transmission
	if(error == 0){
		uint8_t sent = 1;
		while(sent < txBufferLength){
			TWI_WriteByte(twi, txBuffer[sent++]);
			
			if (!TWI_WaitByteSent(twi, XMIT_TIMEOUT))
				error = 3;	// error, got NACK during data transmit
		}
	}
		
	//Send the stop condition
	if(error == 0) {
		TWI_Stop(twi);
		
		if (!TWI_WaitTransferComplete(twi, XMIT_TIMEOUT))
			error = 4;	//Error, finishing up
	}
	
	txBufferLength = 0; //Empty the transmit buffer
	status = MASTER_IDLE;
	return error;
}

uint8_t TwoWireClass::requestFrom(uint8_t address, uint8_t quantity){
	if(quantity > I2C_BUFFER_LENGTH)
	quantity = I2C_BUFFER_LENGTH;
	
	uint8_t numRead = 0;
	
	//Sets up registers to set device into Master Read Mode
	TWI_StartRead(twi, address, 0, 0);
	
	//Read loop
	do
	{
		//Send stop condition if next to last byte to be received
		if(numRead + 1 == quantity)
		TWI_SendSTOPCondition(twi);
		
		//Wait for the byte to arrive and then store it
		TWI_WaitByteReceived(twi, RECV_TIMEOUT);
		rxBuffer[numRead++] = TWI_ReadByte(twi);
		
	} while (numRead < quantity);
	
	TWI_WaitTransferComplete(twi, RECV_TIMEOUT);
	
	return numRead;
}

/** Writes a single byte into txBuffer*/
uint8_t TwoWireClass::write(uint8_t data){
	if(status == MASTER_SEND){
		if(txBufferLength >= I2C_BUFFER_LENGTH) //Transmit buffer full?
			return 0;
		
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
		if(svBufferLength >= I2C_BUFFER_LENGTH) //Service buffer full?
			return 0;
		
		svBuffer[svBufferLength++] = data;
		//srvBuff.storeData(data);
		return 1;
	}
}

/** Writes multiple bytes into txBuffer*/
uint8_t TwoWireClass::write(const uint8_t *dataArray, uint8_t quantity){
	if (status == MASTER_SEND) {
		for (uint8_t i = 0; i < quantity; ++i) {
			if (txBufferLength >= I2C_BUFFER_LENGTH) //Transmit buffer full?
			return i;
			txBuffer[txBufferLength++] = dataArray[i];
		}
		} else {
		for (uint8_t i = 0; i < quantity; ++i) {
			if (svBufferLength >= I2C_BUFFER_LENGTH) //Service buffer full?
			return i;
			svBuffer[svBufferLength++] = dataArray[i];
		}
	}
	return quantity;
}

/** Reads byte from rxBuffer*/
uint8_t TwoWireClass::read(){
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

/** Returns how many bytes are available to read from the rxBuffer*/
uint8_t TwoWireClass::availableToRead(){
	return rxBufferLength - rxBufferIndex;
}

/** Assigns user defined receive event function*/
void TwoWireClass::onReceive(void(*function)(int)){
	onReceiveCallback = function;
}

/** Assigns user defined request event function*/
void TwoWireClass::onRequest(void(*function)(void)){
	onRequestCallback = function;
}

/** Interrupt Handler*/
void TwoWireClass::IRQHandler(){
	//Pull in status register information to help inform program of what happened
	uint32_t regStatus = TWI_GetStatus(twi);

	//Was the slave idle and then successfully accessed by a master?
	if(status == SLAVE_IDLE && TWI_STATUS_SVACC(regStatus)){
		//Disable the slave access flag (SVACC)
		TWI_DisableIt(twi, TWI_IDR_SVACC);

		//Enable Receive Ready, General Call Access, No ACK, End of Slave Access,
		//Clock Wait State, and Transmission Completed flags
		TWI_EnableIt(twi, TWI_IER_RXRDY | TWI_IER_GACC | TWI_IER_NACK
		| TWI_IER_EOSACC | TWI_IER_SCL_WS | TWI_IER_TXCOMP);


		// Detect if we should go into RECV or SEND status
		// SVREAD==1 means *master* reading -> SLAVE_SEND pg.743
		if (TWI_STATUS_SVREAD(regStatus)) {
			status = SLAVE_SEND;

			//Call the user attached function (if it exists) to send data 
			if(onRequestCallback)
				onRequestCallback();
		} else {
			status = SLAVE_RECV;
		}
	}

	
	//If the slave is in the process of something else...
	if(status != SLAVE_IDLE){
		//Is the Transmission Complete  && End of Slave Access flag set?
		if(TWI_STATUS_TXCOMP(regStatus) && TWI_STATUS_EOSACC(regStatus)){
			
			//If in Receive Mode and Receive Callback function exists
			if(status == SLAVE_RECV && onReceiveCallback){
				//Copy received data from svBuffer to rxBuffer. This way, more
				//data can be put into the svBuffer while the callback function
				//reads the rxBuffer. Makes sure two pieces of code aren't trying 
				//to read and write to the svBuffer at the same time.
				for(uint8_t i=0; i<svBufferLength; ++i)
					rxBuffer[i] = svBuffer[i];
				
				//Updates the rxBuffer information to reflect the new data
				rxBufferIndex = 0;
				rxBufferLength = svBufferLength;

				//Tell the calling program to run
				onReceiveCallback(rxBufferLength);
			}

			//Perform some buffer tidying
			if(svBufferIndex >= svBufferLength){
				svBufferIndex = 0;
				svBufferLength = 0;
			} 


			//Transfer is finished. Reset the interrupts and slave status.
			TWI_EnableIt(twi, TWI_SR_SVACC);
			TWI_DisableIt(twi, TWI_IDR_RXRDY | TWI_IDR_GACC | TWI_IDR_NACK
			| TWI_IDR_EOSACC | TWI_IDR_SCL_WS | TWI_IER_TXCOMP);
			
			status = SLAVE_IDLE;
		}
		
	}
	
	if(status == SLAVE_RECV){
		//Is data ready to be read?
		if(TWI_STATUS_RXRDY(regStatus)){
			//Is the svBuffer full of data?
			if(svBufferLength < I2C_BUFFER_LENGTH)
				svBuffer[svBufferLength++] = TWI_ReadByte(twi);
		}
	}

	if(status == SLAVE_SEND){
		//Is the TX register ready && no previous NACK?
		if(TWI_STATUS_TXRDY(regStatus) && !TWI_STATUS_NACK(regStatus)){
			uint8_t c = 0;
			
			//Is there still data available to send?
			if(svBufferIndex < svBufferLength){
				c = svBuffer[svBufferIndex++];
			}
// 			if(srvBuff.memUsed()>0)
// 				c = srvBuff.readData();
			TWI_WriteByte(twi, c);
		}
	}
}

/** Initializes I2C hardware*/
void TwoWireClass::setupHardware(){
	//Enable peripheral clock
	pmc_enable_periph_clk(periphID);
	
	//Pins are defined in hardware as Input/Output, open drain. Only need to switch peripherals.
	PIO_configurePin(
	pinCharacteristic[SDA].port,
	pinCharacteristic[SDA].pinMask,
	pinCharacteristic[SDA].peripheralType,
	pinCharacteristic[SDA].pinAttribute, 0x20);
	
	PIO_configurePin(
	pinCharacteristic[SCL].port,
	pinCharacteristic[SCL].pinMask,
	pinCharacteristic[SCL].peripheralType,
	pinCharacteristic[SCL].pinAttribute, 0x20);
	
	//Disable all PDC channel requests
	twi->TWI_PTCR = TWI_PTCR_RXTDIS | TWI_PTCR_TXTDIS;
	
	//Interrupt handling
	NVIC_DisableIRQ(IRQnID);
	NVIC_ClearPendingIRQ(IRQnID);
	NVIC_SetPriority(IRQnID, 0);
	NVIC_EnableIRQ(IRQnID);
}

uint8_t TwoWireClass::testFunc(){
	return TWI_ReadByte(twi);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Configures a TWI peripheral to operate in master mode, at the given
 * frequency (in Hz). The duty cycle of the TWI clock is set to 50%.
 * \param pTwi  Pointer to an Twi instance.
 * \param twck  Desired TWI clock frequency.
 * \param mck  Master clock frequency.
 */
void TWI_ConfigureMaster( Twi* pTwi, uint32_t dwTwCk, uint32_t dwMCk )
{
	/* Reset the TWI */
	pTwi->TWI_CR = TWI_CR_SWRST;

	/* TWI slave & master mode disabled */
	pTwi->TWI_CR = TWI_CR_SVDIS;
	pTwi->TWI_CR = TWI_CR_MSDIS;
	
	/* Program an arbitrary slave address: pg.713*/
	pTwi->TWI_MMR = 0;
	pTwi->TWI_MMR = (0x33<<16);

	/* Configure clock*/
	TWI_SetClock(pTwi, dwTwCk, dwMCk);
	
	/* Ensure MSDIS not = 1: pg.736*/
	pTwi->TWI_CR = 0;

	/* Set master mode */
	pTwi->TWI_CR = TWI_CR_MSEN;
}

void TWI_SetClock( Twi *pTwi, uint32_t dwTwCk, uint32_t dwMCk )
{
    uint32_t dwCkDiv = 0 ;
    uint32_t dwClDiv ;
    uint32_t dwOk = 0 ;

    /* Configure clock */
    while ( !dwOk )
    {
        dwClDiv = ((dwMCk / (2 * dwTwCk)) - 4) / (1<<dwCkDiv) ;

        if ( dwClDiv <= 255 )
        {
            dwOk = 1 ;
        }
        else
        {
            dwCkDiv++ ;
        }
    }

    pTwi->TWI_CWGR = 0 ;
    pTwi->TWI_CWGR = (dwCkDiv << 16) | (dwClDiv << 8) | dwClDiv ;
}

/**
 * \brief Configures a TWI peripheral to operate in slave mode.
 * \param pTwi  Pointer to an Twi instance.
 * \param slaveAddress Slave address.
 */
void TWI_ConfigureSlave(Twi *pTwi, uint8_t slaveAddress)
{
    //uint32_t i;

    /* TWI software reset */
    pTwi->TWI_CR = TWI_CR_SWRST;
    pTwi->TWI_RHR;

    /* Wait at least 10 ms */
    //for (i=0; i < 1000000; i++);

	/* Configure slave address. */
	pTwi->TWI_SMR = 0;
	pTwi->TWI_SMR = TWI_SMR_SADR(slaveAddress);

    /* TWI Slave Mode Disabled, TWI Master Mode Disabled*/
    pTwi->TWI_CR = TWI_CR_SVDIS | TWI_CR_MSDIS;

	/* Clear the SVDIS bit so the slave can be enabled: pg.737*/
	pTwi->TWI_CR = 0;

    /* SVEN: TWI Slave Mode Enabled */
    pTwi->TWI_CR = TWI_CR_SVEN;

    /* Wait at least 10 ms */
    //for (i=0; i < 1000000; i++);
}

/**
 * \brief Sends a START condition on the TWI.
 * \param pTwi  Pointer to an Twi instance.
 */
void TWI_Start(Twi *pTwi){
	pTwi->TWI_CR |= TWI_CR_START;
}

/**
 * \brief Sends a STOP condition on the TWI.
 * \param pTwi  Pointer to an Twi instance.
 */
void TWI_Stop( Twi *pTwi )
{
    pTwi->TWI_CR = TWI_CR_STOP;
}

/**
 * \brief Starts a read operation on the TWI bus with the specified slave, it returns
 * immediately. Data must then be read using TWI_ReadByte() whenever a byte is
 * available (poll using TWI_ByteReceived()).
 * \param pTwi  Pointer to an Twi instance.
 * \param address  Slave address on the bus.
 * \param iaddress  Optional internal address bytes.
 * \param isize  Number of internal address bytes.
 */
void TWI_StartRead(
    Twi *pTwi,
    uint8_t address,
    uint32_t iaddress,
    uint8_t isize)
{
  
    /* Set slave address and number of internal address bytes. */
    pTwi->TWI_MMR = 0;
    pTwi->TWI_MMR = (isize << 8) | TWI_MMR_MREAD | (address << 16);

    /* Set internal address bytes */
    pTwi->TWI_IADR = 0;
    pTwi->TWI_IADR = iaddress;

    /* Send START condition */
    pTwi->TWI_CR |= TWI_CR_START;
}

/**
 * \brief Reads a byte from the TWI bus. The read operation must have been started
 * using TWI_StartRead() and a byte must be available (check with TWI_ByteReceived()).
 * \param pTwi  Pointer to an Twi instance.
 * \return byte read.
 */
uint8_t TWI_ReadByte(Twi *pTwi)
{
    return pTwi->TWI_RHR;
}

/**
 * \brief Sends a byte of data to one of the TWI slaves on the bus.
 * \note This function must be called once before TWI_StartWrite() with
 * the first byte of data  to send, then it shall be called repeatedly
 * after that to send the remaining bytes.
 * \param pTwi  Pointer to an Twi instance.
 * \param byte  Byte to send.
 */
void TWI_WriteByte(Twi *pTwi, uint8_t byte)
{
    pTwi->TWI_THR = byte;
}

/**
 * \brief Starts a write operation on the TWI to access the selected slave, then
 *  returns immediately. A byte of data must be provided to start the write;
 * other bytes are written next.
 * after that to send the remaining bytes.
 * \param pTwi  Pointer to an Twi instance.
 * \param address  Address of slave to access on the bus.
 * \param iaddress  Optional slave internal address.
 * \param isize  Number of internal address bytes.
 * \param byte  First byte to send.
 */
void TWI_StartWrite(
    Twi *pTwi,
    uint8_t address,
    uint32_t iaddress,
    uint8_t isize,
    uint8_t byte)
{

    /* Set slave address, number of internal address bytes, and transfer direction.*/
	pTwi->TWI_MMR = 0;
    pTwi->TWI_MMR = (isize << 8) | (address << 16);
	
	
    /* Set internal address bytes.*/
    pTwi->TWI_IADR = 0;
    pTwi->TWI_IADR = iaddress;
	
    /* Write first byte to send.*/
    pTwi->TWI_THR = byte;
}

/**
 * \brief Check if a byte have been received from TWI.
 * \param pTwi  Pointer to an Twi instance.
 * \return 1 if a byte has been received and can be read on the given TWI
 * peripheral; otherwise, returns 0. This function resets the status register.
 */
uint8_t TWI_ByteReceived(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY);
}

/**
 * \brief Check if a byte have been sent to TWI.
 * \param pTwi  Pointer to an Twi instance.
 * \return 1 if a byte has been sent so another one can be stored for
 * transmission; otherwise returns 0. This function clears the status register.
 */
uint8_t TWI_ByteSent(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY);
}

/**
 * \brief Check if current transmission is complete.
 * \param pTwi  Pointer to an Twi instance.
 * \return  1 if the current transmission is complete (the STOP has been sent);
 * otherwise returns 0.
 */
uint8_t TWI_TransferComplete(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP);
}

/**
 * \brief Enables the selected interrupts sources on a TWI peripheral.
 * \param pTwi  Pointer to an Twi instance.
 * \param sources  Bitwise OR of selected interrupt sources.
 */
void TWI_EnableIt(Twi *pTwi, uint32_t sources)
{
    pTwi->TWI_IER = sources;
}

/**
 * \brief Disables the selected interrupts sources on a TWI peripheral.
 * \param pTwi  Pointer to an Twi instance.
 * \param sources  Bitwise OR of selected interrupt sources.
 */
void TWI_DisableIt(Twi *pTwi, uint32_t sources)
{
    pTwi->TWI_IDR = sources;
}

/**
 * \brief Get the current status register of the given TWI peripheral.
 * \note This resets the internal value of the status register, so further
 * read may yield different values.
 * \param pTwi  Pointer to an Twi instance.
 * \return  TWI status register.
 */
uint32_t TWI_GetStatus(Twi *pTwi)
{
    return pTwi->TWI_SR;
}

/**
 * \brief Returns the current status register of the given TWI peripheral, but
 * masking interrupt sources which are not currently enabled.
 * \note This resets the internal value of the status register, so further
 * read may yield different values.
 * \param pTwi  Pointer to an Twi instance.
 */
uint32_t TWI_GetMaskedStatus(Twi *pTwi)
{
    uint32_t status;

    status = pTwi->TWI_SR;
    status &= pTwi->TWI_IMR;

    return status;
}

/**
 * \brief  Sends a STOP condition. STOP Condition is sent just after completing
 *  the current byte transmission in master read mode.
 * \param pTwi  Pointer to an Twi instance.
 */
void TWI_SendSTOPCondition(Twi *pTwi)
{
    pTwi->TWI_CR |= TWI_CR_STOP;
}


/*-------------------------------
Status Register Check Functions
---------------------------------*/
bool TWI_STATUS_SVREAD(uint32_t status){
	return (status & TWI_SR_SVREAD) == TWI_SR_SVREAD;
}

bool TWI_STATUS_SVACC(uint32_t status){
	return (status & TWI_SR_SVACC) == TWI_SR_SVACC;
}

bool TWI_STATUS_GACC(uint32_t status){
	return (status & TWI_SR_GACC) == TWI_SR_GACC;
}

bool TWI_STATUS_EOSACC(uint32_t status){
	return (status & TWI_SR_EOSACC) == TWI_SR_EOSACC;
}

bool TWI_STATUS_NACK(uint32_t status){
	return (status & TWI_SR_NACK) == TWI_SR_NACK;
}


/*---------------------------
Timeout Functions (Blocking)
-----------------------------*/

bool TWI_FailedAcknowledge(Twi *pTwi) {
	return pTwi->TWI_SR & TWI_SR_NACK;
}

bool TWI_WaitTransferComplete(Twi *_twi, uint32_t _timeout) {
	uint32_t _status_reg = 0;
	while ((_status_reg & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {
		_status_reg = TWI_GetStatus(_twi);

		if (_status_reg & TWI_SR_NACK)
		return false;

		if (--_timeout == 0)
		return false;
	}
	return true;
}

bool TWI_WaitByteSent(Twi *_twi, uint32_t _timeout) {
	uint32_t _status_reg = 0;
	while ((_status_reg & TWI_SR_TXRDY) != TWI_SR_TXRDY) {
		_status_reg = TWI_GetStatus(_twi);

		if (_status_reg & TWI_SR_NACK)
		return false;

		if (--_timeout == 0)
		return false;
	}

	return true;
}

bool TWI_WaitByteReceived(Twi *_twi, uint32_t _timeout) {
	uint32_t _status_reg = 0;
	while ((_status_reg & TWI_SR_RXRDY) != TWI_SR_RXRDY) {
		_status_reg = TWI_GetStatus(_twi);

		if (_status_reg & TWI_SR_NACK)
		return false;

		if (--_timeout == 0)
		return false;
	}

	return true;
}