/*
 * UART.h
 *
 * Created: 12/13/2015 12:22:03 PM
 *  Author: Brandon
 */ 

/*
	This library controls the UART of the SAM3X8E MCU. On the UDOO board,
	these are digital pins 0 and 1, which correspond to RX0 and TX0 respectively.
	
	The other TX/RX lines on the board are controlled by the USART library.
*/

#ifndef UART_H_
#define UART_H_

#include "../libraries.h"

//Calculates clock divisor (CD) field for UART_BRGR
#define clockDivisor(baud) 84000000L/(16*baud)

class UARTClass
{
public:
	UARTClass(RingBufferClass *pRx_buffer, RingBufferClass *pTx_buffer);

	void	begin();
	void	begin(uint32_t baudRate);
	void	begin(uint32_t baudRate, uint32_t mode);
	void	write(uint8_t val);
	uint8_t read();
	
	void	writeBuff(uint8_t *buffer, uint32_t size);
	void	readBuffer(uint8_t *buffer, uint32_t size);
	
	uint8_t RX_availableToWrite();
	uint8_t RX_availableToRead();
	uint8_t TX_availableToWrite();
	uint8_t TX_availableToRead();
	
	void IrqHandler(void);
protected:
private:
	RingBufferClass *rxBuffer;
	RingBufferClass *txBuffer;
};

#endif /* UART_H_ */