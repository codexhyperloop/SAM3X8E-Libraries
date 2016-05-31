/*
 * spi.cpp
 *
 * Created: 11/23/2015 8:15:35 PM
 *  Author: Brandon
 */ 


/** \addtogroup spi_module Working with SPI
 * The SPI driver provides the interface to configure and use the SPI
 * peripheral.
 *
 * The Serial Peripheral Interface (SPI) circuit is a synchronous serial
 * data link that provides communication with external devices in Master
 * or Slave Mode.
 *
 * To use the SPI, the user has to follow these few steps:
 * -# Enable the SPI pins required by the application (see pio.h).
 * -# Configure the SPI using the \ref SPI_Configure(). This enables the
 *    peripheral clock. The mode register is loaded with the given value.
 * -# Configure all the necessary chip selects with \ref SPI_ConfigureNPCS().
 * -# Enable the SPI by calling \ref SPI_Enable().
 * -# Send/receive data using \ref SPI_Write() and \ref SPI_Read(). Note that \ref SPI_Read()
 *    must be called after \ref SPI_Write() to retrieve the last value read.
 * -# Send/receive data using the PDC with the \ref SPI_WriteBuffer() and
 *    \ref SPI_ReadBuffer() functions.
 * -# Disable the SPI by calling \ref SPI_Disable().
 *
 * For more accurate information, please look at the SPI section of the
 * Datasheet.

*/

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "../libraries.h"

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Enables a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 */
extern void SPI_Enable( Spi* spi )
{
    spi->SPI_CR = SPI_CR_SPIEN ;
}

/**
 * \brief Disables a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 */
extern void SPI_Disable( Spi* spi )
{
    spi->SPI_CR = SPI_CR_SPIDIS ;
}

/**
 * \brief Enables one or more interrupt sources of a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 * \param sources Bitwise OR of selected interrupt sources.
 */
extern void SPI_EnableIt( Spi* spi, uint32_t dwSources )
{
    spi->SPI_IER = dwSources ;
}

/**
 * \brief Disables one or more interrupt sources of a SPI peripheral.
 *
 * \param spi  Pointer to an Spi instance.
 * \param sources Bitwise OR of selected interrupt sources.
 */
extern void SPI_DisableIt( Spi* spi, uint32_t dwSources )
{
    spi->SPI_IDR = dwSources ;
}

/**
 * \brief Configures a SPI peripheral as specified. The configuration can be computed
 * using several macros (see \ref spi_configuration_macros).
 *
 * \param spi  Pointer to an Spi instance.
 * \param id   Peripheral ID of the SPI.
 * \param configuration  Value of the SPI configuration register.
 */
extern void SPI_Configure( Spi* spi, uint32_t id, uint32_t configuration )
{
    //Enable clock for the SPI0 peripheral
    pmc_enable_periph_clk(id);
	
	//Disable SPI instance so it can be reconfigured
	SPI_Disable(spi);
    
	/* Execute a software reset of the SPI twice */
    spi->SPI_CR = SPI_CR_SWRST;
    //spi->SPI_CR = SPI_CR_SWRST;
    spi->SPI_MR = configuration;
}


/**
 * \brief Configures a chip select of a SPI peripheral. The chip select configuration
 * is computed using several macros (see \ref spi_configuration_macros). Each chip select
 * has it's own individual settings and this function will turn them on. External variables
 * must hold the proper settings for each chip select. See setDataMode() and setClockDivider().
 *
 * \param spi   Pointer to an Spi instance.
 * \param npcs  Chip select to configure (0, 1, 2 or 3).
 * \param configuration  Desired chip select configuration.
 */
extern void SPI_ConfigureNPCS( Spi* spi, uint32_t npcs, uint32_t configuration )
{
    spi->SPI_CSR[npcs] = configuration ;
}


/**
 * \brief Get the current status register of the given SPI peripheral.
 * \note This resets the internal value of the status register, so further
 * read may yield different values.
 * \param spi   Pointer to a Spi instance.
 * \return  SPI status register.
 */
extern uint32_t SPI_GetStatus( Spi* spi )
{
    return spi->SPI_SR ;
}

/**
 * \brief Reads and returns the last word of data received by a SPI peripheral. This
 * method must be called after a successful SPI_Write call.
 *
 * \param spi  Pointer to an Spi instance.
 *
 * \return readed data.
 */
extern uint32_t SPI_Read( Spi* spi )
{
    while ( (spi->SPI_SR & SPI_SR_RDRF) == 0 ) ;

    return spi->SPI_RDR & 0xFFFF ;
}

/**
 * \brief Sends data through a SPI peripheral. If the SPI is configured to use a fixed
 * peripheral select, the npcs value is meaningless. Otherwise, it identifies
 * the component which shall be addressed.
 *
 * \param spi   Pointer to an Spi instance.
 * \param npcs  Chip select of the component to address (0, 1, 2 or 3).
 * \param data  Word of data to send.
 */
extern void SPI_Write( Spi* spi, uint32_t npcs, uint16_t data )
{
    /* Send data */
    while ( (spi->SPI_SR & SPI_SR_TXEMPTY) == 0 ) ;
    spi->SPI_TDR = data | SPI_PCS(npcs) ;
    while ( (spi->SPI_SR & SPI_SR_TDRE) == 0 ) ;
}

/**
 * \brief Check if SPI transfer finish.
 *
 * \param spi  Pointer to an Spi instance.
 *
 * \return Returns 1 if there is no pending write operation on the SPI; otherwise
 * returns 0.
 */
extern uint32_t SPI_IsFinished( Spi* spi )
{
    return ((spi->SPI_SR & SPI_SR_TXEMPTY) != 0) ;
}

/************************************************************************/
/* User level SPI Functions                                             */
/************************************************************************/

/** 
To use the SPI, the user has to follow these few steps:
* -# Enable the SPI pins required by the application (see pio.h).
* -# Configure the SPI using the \ref SPI_Configure(). This enables the
*    peripheral clock. The mode register is loaded with the given value.
* -# Configure all the necessary chip selects with \ref SPI_ConfigureNPCS().
* -# Enable the SPI by calling \ref SPI_Enable().
* -# Send/receive data using \ref SPI_Write() and \ref SPI_Read(). Note that \ref SPI_Read()
*    must be called after \ref SPI_Write() to retrieve the last value read.
* -# Send/receive data using the PDC with the \ref SPI_WriteBuffer() and
*    \ref SPI_ReadBuffer() functions.
* -# Disable the SPI by calling \ref SPI_Disable().
*/


extern void SPIClass::begin(uint8_t numChannels){
	//If SPI hardware has already been initialized, exit.
	if(initialized)
	return;
	
	interruptMode = 0;
	interruptSave = 0;
	interruptMask[0] = 0;
	interruptMask[1] = 0;
	interruptMask[2] = 0;
	//interruptMask[3] = 0;
	
	this->init_SPI_PIO(numChannels);
	this->init_SPI(numChannels);
}

extern void SPIClass::end(){
	SPI_Disable(SPI0);
	initialized = false; 
}

extern void SPIClass::setBitOrder(uint32_t pin, uint32_t bitOrder){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->bitOrder[channel] = bitOrder;
}

extern void SPIClass::setClockDivider(uint32_t pin, uint32_t clockDiv){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->clockDivider[channel] = clockDiv;
}

extern void SPIClass::setClockFrequency(uint32_t pin, uint32_t clockFreq){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->clockFrequency[channel] = clockFreq;
}

extern void SPIClass::setDataMode(uint32_t pin, uint32_t mode){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->spiMode[channel] = mode;
}

extern void SPIClass::setInterruptMask(uint32_t pin, uint32_t mask){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->interruptMask[channel] = mask;
}

extern void SPIClass::setDelayAfterTrans(uint32_t pin, uint32_t delayVal){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->delayAfterTransfer[channel] = delayVal;
}

extern void SPIClass::setDelayBeforeTrans(uint32_t pin, uint32_t delayVal){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->delayBeforeTransfer[channel] = delayVal;
}

extern void SPIClass::setMessageLength(uint32_t pin, uint32_t length){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	this->messageLength[channel] = length;
}

extern void SPIClass::updateSPISettings(uint32_t pin){
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	
	//Disable SPI so you can update the settings
	SPI_Disable(SPI0);
	
	//Create a configuration based on channel select
	uint32_t csrConfig = 
		clockDivider[channel] |
		//clockFrequency[channel] |
		messageLength[channel] |
		spiMode[channel] |
		interruptMask[channel] |
		delayBeforeTransfer[channel] |
		delayAfterTransfer[channel];
	
	//Write configuration 
	SPI_ConfigureNPCS(SPI0, channel, csrConfig);
	
	//Reenable SPI
	SPI_Enable(SPI0);

}

extern void SPIClass::write(uint32_t pin, uint16_t data){
	//Wait for previous transfer to complete
	while ((SPI0->SPI_SR & SPI_SR_TXEMPTY) == 0);
	
	uint32_t channel = BOARD_PIN_TO_SPI_CHANNEL(pin);
	uint32_t pcs = SPI_PCS(channel);
	
	//Load the Transmit Data Register with data and which peripheral to send it to
	SPI0->SPI_TDR = data|pcs;
	
	//Wait for data to be transferred to serializer
	while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
}

extern uint32_t SPIClass::read(){
	uint32_t channel, readData, pcs;
	
	//Wait for a previous transfer to complete
	while ( (SPI0->SPI_SR & SPI_SR_RDRF) == 0 ) ;
	readData = SPI0->SPI_RDR & 0xFFFF;
	
	//Grabs the information of which peripheral data was just read from
	pcs = ((SPI0->SPI_RDR & 0xFFFFF) >> 16) & 0xF;
	
	//Extract channel number
	channel = PCS_TO_CHANNEL(pcs);
	
	//Write the new data to the buffer
	readBuff[channel] = readData;
	
	return readData;
}

/************************************************************************/
/* Private Functions                                                    */
/************************************************************************/
extern void SPIClass::init_SPI_PIO(uint8_t numChannels){
	//Enable peripheral clock for PIOA
	pmc_enable_periph_clk(ID_PIOA);
	
	//Configure MISO for peripheral A and as an input
	PIO_configurePin(
		pinCharacteristic[MISO].port,
		pinCharacteristic[MISO].pinMask,
		pinCharacteristic[MISO].peripheralType,
		pinCharacteristic[MISO].pinAttribute,
		INPUT);
	
	//Configure MOSI for peripheral A and as an output
	PIO_configurePin(
		pinCharacteristic[MOSI].port,
		pinCharacteristic[MOSI].pinMask,
		pinCharacteristic[MOSI].peripheralType,
		pinCharacteristic[MOSI].pinAttribute,
		OUTPUT);
	
	//Configure SCK for peripheral A and as an output
	PIO_configurePin(
		pinCharacteristic[SCK].port,
		pinCharacteristic[SCK].pinMask,
		pinCharacteristic[SCK].peripheralType,
		pinCharacteristic[SCK].pinAttribute,
		OUTPUT);
	
	//Configure all needed Chip Selects
	for(uint8_t x=0; x<numChannels; ++x){
		switch (x)
		{
		case 0:
			//Configure CS0
			PIO_configurePin(
			pinCharacteristic[77].port,
			pinCharacteristic[77].pinMask,
			pinCharacteristic[77].peripheralType,
			pinCharacteristic[77].pinAttribute,
			OUTPUT);
			break;
		
		case 1:
			//Configure CS1 
			PIO_configurePin(
			pinCharacteristic[86].port,
			pinCharacteristic[86].pinMask,
			pinCharacteristic[86].peripheralType,
			pinCharacteristic[86].pinAttribute,
			OUTPUT);
			break;
		
		case 2:
			//Configure CS2
			PIO_configurePin(
			pinCharacteristic[87].port,
			pinCharacteristic[87].pinMask,
			pinCharacteristic[87].peripheralType,
			pinCharacteristic[87].pinAttribute,
			OUTPUT);
			break;
		
		default: return;
		}
	}
	
}

extern void SPIClass::init_SPI(uint8_t numChannels){
	SPI_Disable(SPI0);
	
	//Default spi instance configuration options
	uint32_t spiConfiguration = 
	SPI_MR_MSTR|		//Sets SPI to master mode
	SPI_MR_MODFDIS|		//Disables mode fault detection
	SPI_MR_PS;			//Sets SPI to use variable peripheral selects
	
	//Enable the above configuration options
	SPI_Configure(SPI0, ID_SPI0, spiConfiguration);
	
	
	//Initialize all channel configurations to default
	for(uint8_t x=0; x<numChannels; ++x){
		//Make sure we don't go outside supported device channels
		if(numChannels>NUM_SPI_CHANNELS)
			x = numChannels+1;
		
		bitOrder[x] = MSBFIRST;
		messageLength[x] = SPI_8BIT;
		clockDivider[x] = SPI_CLOCK_DIV16;
		spiMode[x] = SPI_MODE0|SPI_CSNAAT;
		clockFrequency[x] = 4000000L;
		interruptMask[x] = 0x00000000;
		delayBeforeTransfer[x] = SPI_DLYBS(0, mstrClock);
		delayAfterTransfer[x] = SPI_DLYBCT(0, mstrClock);
	}
	
	//Default chip select configuration options
	uint32_t csrConfiguration = 
	SPI_CLOCK_FREQ(4000000L)|	//Macro calculates SCBR value make clock run at 4MHz
	SPI_8BIT|					//Defines an 8 bit transfer
	SPI_MODE0|					//Sets SPI to use Mode 0, pg. 680 of datasheet
	SPI_CSNAAT|					//Tells Chip Select to Not be Active After Transfer (CSNAAT)
	SPI_DLYBCT(0, mstrClock)| //Defines the delay before disabling the chip select
	SPI_DLYBS(0, mstrClock);	//Defines the delay before starting a transfer
	
	SPI_ConfigureNPCS(SPI0, 0, csrConfiguration);
	 
	//Enable the SPI0 unit
	SPI_Enable(SPI0);
}