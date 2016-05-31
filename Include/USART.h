
#ifndef _USART_
#define _USART_

/************************************************************************/
/* Headers                                                              */
/************************************************************************/
#include "../libraries.h"

//Calculates clock divisor (CD) field for USART_BRGR
#define clockDivisor(baud) 84000000L/(16*baud)


class USARTClass{
public:
	RingBufferClass *rxBuffer;
	RingBufferClass *txBuffer;

	void	begin();
	void	begin(uint32_t baudRate);
	void	begin(uint32_t baudRate, uint32_t mode);
	void	write(uint8_t val);
	uint8_t	read();
	
	void	writeBuff(uint8_t *buffer, uint32_t size);
	void	readBuffer(uint8_t *buffer, uint32_t size);
	
	uint8_t RX_availableToWrite();
	uint8_t RX_availableToRead();
	uint8_t TX_availableToWrite();
	uint8_t TX_availableToRead();
		
	/** Constructor*/
	USARTClass(uint32_t channel, RingBufferClass *rxbuff, RingBufferClass *txbuff);
	void init();
	
	
	//Interrupt Handler
	void IrqHandler(void);
	
private:
//Instance and pin definitions
	Usart *instance;
	IRQn_Type IRQNum;
	uint32_t instanceID, instanceMode;
	uint32_t rxPin, txPin, baud, initialized;
};


/************************************************************************/
/* Exported Functions                                                   */
/************************************************************************/
extern void USART_Configure(Usart *usartInstance, uint32_t periphID, IRQn_Type IRQID, uint32_t mode, uint32_t baudRate);


#endif /* #ifndef _USART_ */