/**
 * \file
 *
 * Interface for configuration the Two Wire Interface (TWI) peripheral.
 *
 */

#ifndef _TWI_
#define _TWI_

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "../libraries.h"

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/
/* Returns 1 if the TXRDY bit (ready to transmit data) is set in the given status register value.*/
#define TWI_STATUS_TXRDY(status) (((status) & TWI_SR_TXRDY) == TWI_SR_TXRDY)

/* Returns 1 if the RXRDY bit (ready to receive data) is set in the given status register value.*/
#define TWI_STATUS_RXRDY(status) (((status) & TWI_SR_RXRDY) == TWI_SR_RXRDY)

/* Returns 1 if the TXCOMP bit (transfer complete) is set in the given status register value.*/
#define TWI_STATUS_TXCOMP(status) (((status) & TWI_SR_TXCOMP) == TWI_SR_TXCOMP)


/*----------------------------------------------------------------------------
 *        External function
 *----------------------------------------------------------------------------*/

extern void TWI_ConfigureMaster(Twi *pTwi, uint32_t twck, uint32_t mck);

extern void TWI_SetClock( Twi *pTwi, uint32_t dwTwCk, uint32_t dwMCk );

extern void TWI_ConfigureSlave(Twi *pTwi, uint8_t slaveAddress);

extern void TWI_Start(Twi *pTwi);

extern void TWI_Stop(Twi *pTwi);

extern void TWI_StartRead(Twi *pTwi, uint8_t address, uint32_t iaddress, uint8_t isize);

extern uint8_t TWI_ReadByte(Twi *pTwi);

extern void TWI_WriteByte(Twi *pTwi, uint8_t byte);

extern void TWI_StartWrite(Twi *pTwi, uint8_t address, uint32_t iaddress, uint8_t isize, uint8_t byte);

extern uint8_t TWI_ByteReceived(Twi *pTwi);

extern uint8_t TWI_ByteSent(Twi *pTwi);

extern uint8_t TWI_TransferComplete(Twi *pTwi);

extern void TWI_EnableIt(Twi *pTwi, uint32_t sources);

extern void TWI_DisableIt(Twi *pTwi, uint32_t sources);

extern uint32_t TWI_GetStatus(Twi *pTwi);

extern uint32_t TWI_GetMaskedStatus(Twi *pTwi);

extern void TWI_SendSTOPCondition(Twi *pTwi);

extern bool TWI_STATUS_SVREAD(uint32_t status);

extern bool TWI_STATUS_SVACC(uint32_t status);

extern bool TWI_STATUS_GACC(uint32_t status);

extern bool TWI_STATUS_EOSACC(uint32_t status);

extern bool TWI_STATUS_NACK(uint32_t status);

extern bool TWI_FailedAcknowledge(Twi *pTwi);

extern bool TWI_WaitTransferComplete(Twi *_twi, uint32_t _timeout);

extern bool TWI_WaitByteSent(Twi *_twi, uint32_t _timeout);

extern bool TWI_WaitByteReceived(Twi *_twi, uint32_t _timeout);


#define I2C_BUFFER_LENGTH 32

class TwoWireClass{
public:
	TwoWireClass(Twi *pTwi);
	
	void begin();
	void begin(uint8_t address);
	void setMaster();
	void setSlave();
	void beginTransmission(uint8_t address);
	uint8_t endTransmission();
	uint8_t write(uint8_t value);
	uint8_t write(const uint8_t *dataArray, uint8_t quantity);
	uint8_t read();
	uint8_t availableToRead();
	uint8_t requestFrom(uint8_t address, uint8_t quantity);

	//Allows a user to attach a function to an event
	void onRequest(void(*)(void));
	void onReceive(void(*)(int));
	
	uint8_t testFunc();
	
	//Interrupt Handler
	void IRQHandler(void);

private:
	
	//Instance specific information
	Twi *twi;
	IRQn_Type IRQnID;
	uint8_t periphID;
	uint8_t SDA, SCL;
	uint8_t txAddress;
	
	//TWI status
	enum TwoWireStatus {
		UNINITIALIZED,
		MASTER_IDLE,
		MASTER_SEND,
		MASTER_RECV,
		SLAVE_IDLE,
		SLAVE_RECV,
		SLAVE_SEND
	};
	//Declare as volatile to enable interrupt to "see" variable
	volatile TwoWireStatus status;
	
	//TWI clock frequency
	static const uint32_t TWI_CLOCK = 100000;
	uint32_t twiClock;

	//Timeouts
	static const uint32_t RECV_TIMEOUT = 1000;
	static const uint32_t XMIT_TIMEOUT = 1000;
	
	//Data Buffers
	uint8_t txBuffer[I2C_BUFFER_LENGTH];	//Transmit Data (Master Mode)
	uint8_t rxBuffer[I2C_BUFFER_LENGTH];	//Receive Data (Master Mode)
	uint8_t svBuffer[I2C_BUFFER_LENGTH];	//Stored Data (Slave Mode)

	//RingBufferClass srvBuff;
	
	//Data Buffer Length Variables (how full is buffer?)
	uint8_t txBufferLength;
	uint8_t rxBufferLength;
	uint8_t svBufferLength;
	
	//Data Buffer Index Variables (where in buffer currently?)
	uint8_t rxBufferIndex;
	uint8_t svBufferIndex;
	
	//Callback user functions
	void (*onRequestCallback)(void);
	void (*onReceiveCallback)(int);

	void setupHardware();
};

#endif /* #ifndef _TWI_ */