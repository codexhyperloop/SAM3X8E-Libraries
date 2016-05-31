//TX3 & RX3 are USART3 -> Serial3
//TX2 & RX2 are USART1 -> Serial2
//TX1 & RX1 are USART0 -> Serial1


#include "../libraries.h"


/************************************************************************/
/* Constructor	                                                        */
/************************************************************************/
USARTClass::USARTClass(uint32_t channel, RingBufferClass *rxbuff, RingBufferClass *txbuff){

	switch(channel)
	{
		case 1:
		instance = USART0;
		instanceID = ID_USART0;
		IRQNum = USART0_IRQn;
		txPin  = 18;
		rxPin  = 19;
		break;
		
		case 2:
		instance = USART1;
		instanceID = ID_USART1;
		IRQNum = USART1_IRQn;
		txPin  = 16;
		rxPin  = 17;
		break;
		
		case 3:
		instance = USART3;
		instanceID = ID_USART3;
		IRQNum = USART3_IRQn;
		txPin  = 14;
		rxPin  = 15;
		break;
		default: return;
	}
	
	txBuffer = txbuff;
	rxBuffer = rxbuff;
	
	initialized = false;
}

void USARTClass::begin(){
	baud = 115200;
	instanceMode = US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK| US_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT|
					US_MR_PAR_NO;
	init();
}

void USARTClass::begin(uint32_t baudRate){
	baud = baudRate;
	instanceMode = US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK| US_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT|
					US_MR_PAR_NO;
	init();
}

void USARTClass::begin(uint32_t baudRate, uint32_t mode){
	baud = baudRate;
	instanceMode = mode;
	init();
}

void USARTClass::init(){
	//Has the USART channel already been initialized?
	if(initialized)
	return;
	
	//Initialize pins and hand control over to peripheral.
	PIO_configurePin(
	pinCharacteristic[txPin].port,
	pinCharacteristic[txPin].pinMask,
	pinCharacteristic[txPin].peripheralType,
	pinCharacteristic[txPin].pinAttribute,
	OUTPUT);
	
	PIO_configurePin(
	pinCharacteristic[rxPin].port,
	pinCharacteristic[rxPin].pinMask,
	pinCharacteristic[rxPin].peripheralType,
	pinCharacteristic[rxPin].pinAttribute,
	INPUT);
	
	USART_Configure(instance, instanceID, IRQNum, instanceMode, baud);
	
	//Lock out this function from further use.
	initialized = true;
}



/************************************************************************/
/* Public Functions                                                     */
/************************************************************************/
/** Write a single byte*/
void USARTClass::write(uint8_t val){
	// Is the hardware currently busy?
	if (((instance->US_CSR & US_CSR_TXRDY) != US_CSR_TXRDY) | (txBuffer->readLocation != txBuffer->writeLocation))
	{
		// If busy we buffer
		unsigned int stopLocation = (txBuffer->writeLocation + 1) % BUFFER_SIZE;
		while (txBuffer->readLocation == stopLocation)
		; // Spin locks if we're about to overwrite the buffer. This continues once the data is sent

		txBuffer->ringBuffer[txBuffer->writeLocation] = val;
		txBuffer->writeLocation = stopLocation;
		
		// Make sure TX interrupt is enabled
		instance->US_IER = US_IER_TXRDY;
	}
	else
	{
		// Bypass buffering and send character directly
		instance->US_THR = val;
	}
}

/** Read a single byte*/
uint8_t USARTClass::read(){
	// if the head isn't ahead of the tail, we don't have any characters
	if (rxBuffer->writeLocation == rxBuffer->readLocation )
		return -1;

	return rxBuffer->readData();
}

uint8_t USARTClass::RX_availableToRead(){
	return rxBuffer->memUsed();
}

uint8_t USARTClass::RX_availableToWrite(){
	return rxBuffer->memAvailable();
}

uint8_t USARTClass::TX_availableToRead(){
	return txBuffer->memUsed();
}

uint8_t USARTClass::TX_availableToWrite(){
	return txBuffer->memAvailable();
}

void USARTClass::IrqHandler(void){
	uint32_t status = instance->US_CSR;
	
	// Did we receive data?
	if ((status & US_CSR_RXRDY) == US_CSR_RXRDY)
	rxBuffer->storeData(instance->US_RHR);

	// Do we need to keep sending data?
	if ((status & US_CSR_TXRDY) == US_CSR_TXRDY)
	{
		if (txBuffer->readLocation != txBuffer->writeLocation) {
			//instance->US_THR = txBuffer->ringBuffer[txBuffer->readLocation];
			//txBuffer->readLocation = (unsigned int)(txBuffer->readLocation + 1) % BUFFER_SIZE;
			instance->US_THR = txBuffer->readData();
		}
		else
		{
			// Mask off transmit interrupt so we don't get it anymore
			instance->US_IDR = US_IDR_TXRDY;
		}
	}

	// Acknowledge errors
	if ((status & US_CSR_OVRE) == US_CSR_OVRE || (status & US_CSR_FRAME) == US_CSR_FRAME)
	{
		// TODO: error reporting outside ISR
		instance->US_CR |= US_CR_RSTSTA;
	}
}


/************************************************************************/
/* Exported Functions                                                   */
/************************************************************************/

void USART_Configure(Usart *usartInstance, uint32_t periphID, IRQn_Type IRQID, uint32_t mode, uint32_t baudRate){
	//Turn on peripheral clock
	pmc_enable_periph_clk(periphID);
	
	//Disable PDC Requests
	usartInstance->US_PTCR = US_PTCR_RXTDIS | US_PTCR_TXTDIS;
	
	//Reset TX, RX & Status Register. Disable TX & RX
	usartInstance->US_CR = US_CR_RSTRX | US_CR_RSTTX | US_CR_RXDIS | US_CR_TXDIS | US_CR_RSTSTA;
	
	//Set the mode: Normal Channel, Master Clock, 8 Bit, and whatever is defined in usartMode
	usartInstance->US_MR = mode;
	
	//Set the Baud Rate
	usartInstance->US_BRGR = clockDivisor(baudRate);
	
	//Disable all interrupts and then config
	usartInstance->US_IDR = 0xFFFFFFFF;
	NVIC_EnableIRQ(IRQID);
	usartInstance->US_IER = US_IER_RXRDY;
	
	//Enable TX & RX
	usartInstance->US_CR &= ~(US_CR_RSTRX | US_CR_RSTTX | US_CR_RXDIS | US_CR_TXDIS | US_CR_RSTSTA);
	usartInstance->US_CR = US_CR_RXEN | US_CR_TXEN;
}