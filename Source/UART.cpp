/*
 * UART.cpp
 *
 * Created: 12/13/2015 12:22:22 PM
 *  Author: Brandon
 */ 

#include "../libraries.h"

//Constructor
UARTClass::UARTClass(RingBufferClass *pRx_buffer, RingBufferClass *pTx_buffer){
	rxBuffer = pRx_buffer;
	txBuffer = pTx_buffer;
}

//Initializes UART to default parameters
void UARTClass::begin(){
	//Enable the peripheral clock to the UART
	pmc_enable_periph_clk(ID_UART);
	
	/** Configure RX0/TX0 through PIO. This also disables PIO control
	of the pins and hands it over to the peripheral controller.*/
	PIO_configurePin(
	pinCharacteristic[RX0].port, 
	pinCharacteristic[RX0].pinMask, 
	pinCharacteristic[RX0].peripheralType,
	pinCharacteristic[RX0].pinAttribute, INPUT);
	
	PIO_configurePin(
	pinCharacteristic[TX0].port,
	pinCharacteristic[TX0].pinMask,
	pinCharacteristic[TX0].peripheralType,
	pinCharacteristic[TX0].pinAttribute, OUTPUT);
	
	// Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	// Reset and disable transmitter and receiver
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	//Set the mode: Normal Channel
	UART->UART_MR = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO;
	
	//Set the baud rate: 115200
	UART->UART_BRGR = clockDivisor(115200);
	
	//Disable all interrupts and then configure for the one you want
	UART->UART_IDR = 0xFFFFFFFF; //Disable all
	NVIC_EnableIRQ(UART_IRQn);
	UART->UART_IER = UART_IER_RXRDY; 
	
	// Enable receiver and transmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

//Initializes UART to default parameters + baudrate
void UARTClass::begin(uint32_t baudRate){
	/** Configure RX0/TX0 through PIO. This also disables PIO control
	of the pins and hands it over to the peripheral controller.*/
	PIO_configurePin(
	pinCharacteristic[RX0].port, 
	pinCharacteristic[RX0].pinMask, 
	pinCharacteristic[RX0].peripheralType,
	pinCharacteristic[RX0].pinAttribute, INPUT);
	
	PIO_configurePin(
	pinCharacteristic[TX0].port,
	pinCharacteristic[TX0].pinMask,
	pinCharacteristic[TX0].peripheralType,
	pinCharacteristic[TX0].pinAttribute, OUTPUT);
	
	//Enable the peripheral clock to the UART
	pmc_enable_periph_clk(ID_UART);
	
	//Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	//Reset and disable transmitter and receiver
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	//Set the mode: Normal Channel
	UART->UART_MR = UART_MR_CHMODE_NORMAL;
	
	//Set the baud rate: 115200
	UART->UART_BRGR = clockDivisor(baudRate);
	
	//Disable all interrupts and then configure for the one you want
	UART->UART_IDR = 0xFFFFFFFF; //Disable all
	NVIC_EnableIRQ(UART_IRQn);
	UART->UART_IER = UART_IER_RXRDY;
	
	//Enable receiver and transmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

//Initializes UART to "mode" parameters + baudrate
void UARTClass::begin(uint32_t baudRate, uint32_t mode){
	/** Configure RX0/TX0 through PIO. This also disables PIO control
	of the pins and hands it over to the peripheral controller.*/
	PIO_configurePin(
	pinCharacteristic[RX0].port, 
	pinCharacteristic[RX0].pinMask, 
	pinCharacteristic[RX0].peripheralType,
	pinCharacteristic[RX0].pinAttribute, INPUT);
	
	PIO_configurePin(
	pinCharacteristic[TX0].port,
	pinCharacteristic[TX0].pinMask,
	pinCharacteristic[TX0].peripheralType,
	pinCharacteristic[TX0].pinAttribute, OUTPUT);
	
	//Enable the peripheral clock to the UART
	pmc_enable_periph_clk(ID_UART);
	
	// Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	// Reset and disable transmitter and receiver
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	//Set the mode: Even parity and Normal Channel
	UART->UART_MR = mode;
	
	//Set the baud rate: 115200
	UART->UART_BRGR = clockDivisor(baudRate);
	
	//Disable all interrupts and then configure for the one you want
	UART->UART_IDR = 0xFFFFFFFF; //Disable all
	NVIC_EnableIRQ(UART_IRQn);
	UART->UART_IER = UART_IER_RXRDY; 
	
	// Enable receiver and transmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

//Writes a single byte 
void UARTClass::write(uint8_t val){
	// Is the hardware currently busy?
	if (((UART->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY) | (txBuffer->readLocation != txBuffer->writeLocation))
	{
		// If busy we buffer
		unsigned int stopLocation = (txBuffer->writeLocation + 1) % BUFFER_SIZE;
		while (txBuffer->readLocation == stopLocation)
		; // Spin locks if we're about to overwrite the buffer. This continues once the data is sent

		txBuffer->ringBuffer[txBuffer->writeLocation] = val;
		txBuffer->writeLocation = stopLocation;
		
		// Make sure TX interrupt is enabled
		UART->UART_IER = UART_IER_TXRDY;
	}
	else
	{
		// Bypass buffering and send character directly
		UART->UART_THR = val;
	}
}

//Reads a single byte
uint8_t UARTClass::read(){
	// if the head isn't ahead of the tail, we don't have any characters
	if ( rxBuffer->writeLocation == rxBuffer->readLocation )
	return -1;

	return rxBuffer->readData();
}
 
uint8_t UARTClass::RX_availableToRead(){
	return rxBuffer->memUsed();
}

uint8_t UARTClass::RX_availableToWrite(){
	return rxBuffer->memAvailable();
}

uint8_t UARTClass::TX_availableToRead(){
	return txBuffer->memUsed();
}

uint8_t UARTClass::TX_availableToWrite(){
	return txBuffer->memAvailable();
}

void UARTClass::IrqHandler(void){
	uint32_t status = UART->UART_SR;
	
	// Did we receive data?
	if ((status & UART_SR_RXRDY) == UART_SR_RXRDY)
	rxBuffer->storeData(UART->UART_RHR);

	// Do we need to keep sending data?
	if ((status & UART_SR_TXRDY) == UART_SR_TXRDY)
	{
		if (txBuffer->readLocation != txBuffer->writeLocation) {
// 			UART->UART_THR = txBuffer->ringBuffer[txBuffer->readLocation];
// 			txBuffer->readLocation = (unsigned int)(txBuffer->readLocation + 1) % BUFFER_SIZE;
			UART->UART_THR = txBuffer->readData();
		}
		else
		{
			// Mask off transmit interrupt so we don't get it anymore
			UART->UART_IDR = UART_IDR_TXRDY;
		}
	}

	// Acknowledge errors
	if ((status & UART_SR_OVRE) == UART_SR_OVRE || (status & UART_SR_FRAME) == UART_SR_FRAME)
	{
		// TODO: error reporting outside ISR
		UART->UART_CR |= UART_CR_RSTSTA;
	}
}