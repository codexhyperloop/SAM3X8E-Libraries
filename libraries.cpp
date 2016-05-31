/*
 * libraries.cpp
 *
 * Created: 12/28/2015 2:58:45 PM
 *  Author: Brandon
 */ 

#include "libraries.h"

/************************************************************************/
/* Global Class Definitions                                             */
/************************************************************************/


/*****************
UART Objects
******************/
RingBufferClass txBuffer1;
RingBufferClass rxBuffer1;

UARTClass Serial(&rxBuffer1, &txBuffer1);

//Interrupt Handler
void UART_Handler(void)
{
	Serial.IrqHandler();
}

/*****************
USART Objects
******************/
RingBufferClass txBuffer2;
RingBufferClass rxBuffer2;
RingBufferClass txBuffer3;
RingBufferClass rxBuffer3;
RingBufferClass txBuffer4;
RingBufferClass rxBuffer4;

USARTClass Serial1(1, &rxBuffer2, &txBuffer2);
USARTClass Serial2(2, &rxBuffer3, &txBuffer3);
USARTClass Serial3(3, &rxBuffer4, &txBuffer4);

//Interrupt Handlers
void USART0_Handler(void){
	Serial1.IrqHandler();
}

void USART1_Handler(void){	Serial2.IrqHandler();
}

void USART3_Handler(void){
	Serial3.IrqHandler();}


/*****************
I2C Objects
******************/

/** Class objects*/
TwoWireClass Wire(TWI1);	//20, 21
TwoWireClass Wire1(TWI0);	//Unlabeled

/** Interrupt Handlers*/
void TWI0_Handler(){ //Wire1
	Wire1.IRQHandler();
};

void TWI1_Handler(){ //Wire
	Wire.IRQHandler();
}

/*****************
SPI Objects
******************/

SPIClass SPI;
