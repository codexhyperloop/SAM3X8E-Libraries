
/**
 * \file
 *
 * Interface for Serial Peripheral Interface (SPI) controller.
 *
 * Currently needs support added for Interrupts, expanded chip
 * select configurations, and variable frequency settings that
 * are not limited to the CLOCK_DIV options.
 */

#ifndef _SPI_
#define _SPI_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "../libraries.h"

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

/**
 *
 * Here are several macros which should be used when configuring a SPI
 * peripheral.
 *
 * \section spi_configuration_macros SPI Configuration Macros
 * - \ref SPI_PCS
 * - \ref SPI_SCBR
 * - \ref SPI_DLYBS
 * - \ref SPI_DLYBCT
 */

#define mstrClock 84000000
//Number of hardwired channels available
#define NUM_SPI_CHANNELS 3
//Number of connected peripherals...functions need to be updated to use this
#define NUM_SPI_PERIPH  10

/************************************************************************/
/*MACROS                                                                */
/************************************************************************/

/** Calculate the Peripheral Chip Select (PCS) field value given the chip select NPCS value
	PG. 695 in Datasheet */
#define SPI_PCS(npcs)       ((~(1 << (npcs)) & 0xF) << 16)

/** Calculates the value of the Chip Select Register (CSR) Serial Clock Baud Rate (SCBR) field given the baudrate and MCK. */
#define SPI_SCBR(baudrate, mstrClock) ((uint32_t) ((mstrClock) / (baudrate)) << 8)

/** Calculates the value of the Chip Select Register (CSR) Delay Before Spi Clock (DLYBS) field given the desired delay (in ns) */
#define SPI_DLYBS(delay, mstrClock) ((uint32_t) ((((mstrClock) / 1000000) * (delay)) / 1000) << 16)

/** Calculates the value of the Chip Select Register (CSR) Delay Between Consecutive Transfers (DLYBCT) field given the desired delay (in ns) */
#define SPI_DLYBCT(delay, mstrClock) ((uint32_t) ((((mstrClock) / 1000000) * (delay)) / 32000) << 24)

/** A set of nested IF functions that determine which hardwired Chip Select the
inputted board pin number belongs to. If it does not fit any of them, it defaults
to the 4th chip select (SS3).*/
#define BOARD_PIN_TO_SPI_PIN(x) \
(x==BOARD_SPI_SS0 ? PIN_SPI_SS0 : \
(x==BOARD_SPI_SS1 ? PIN_SPI_SS1 : \
(x==BOARD_SPI_SS2 ? PIN_SPI_SS2 : PIN_SPI_SS3 )))

/** A set of nested IF functions that determine which channel (Chip Select) the
inputted board pin number belongs to. If it does not fit any of them, it defaults
to the 4th channel (SS3). Channels are necessary to have multiple SPI settings for
possibly different connected SPI devices. It is a matter of convenience.*/
#define BOARD_PIN_TO_SPI_CHANNEL(x) \
(x==BOARD_SPI_SS0 ? 0 : \
(x==BOARD_SPI_SS1 ? 1 : \
(x==BOARD_SPI_SS2 ? 2 : 3)))

/** A set of nested IF functions that determine the channel from PCS.
If it does not fit any of them, choose channel 0.*/
#define PCS_TO_CHANNEL(x) \
(x == 0xE ? 0 : \
(x == 0xD ? 1 : \
(x == 0xB ? 2 : \
(x == 0x7 ? 3 : 0))))

/************************************************************************/
/* General Definitions                                                  */
/************************************************************************/

/** SPI Mode Register Control Bits*/
#define SPI_MSTR	0u		//Master
#define SPI_SLAVE	(1u<<0)	//Slave	
#define SPI_PS		(1u<<1) //Peripheral Select
#define SPI_PCSDEC	(1u<<2)	//Chip Select Decode
#define SPI_MODFDIS (1u<<4)	//Mode Fault Detection
#define SPI_WDRBT	(1u<<5)	//Wait Data Read Before Transfer
#define SPI_LLB		(1u<<7)	//Local Loopback Enable

/** SPI Chip Select Register Control Bits*/
#define SPI_CPOL	(1u<<0)	//Clock Polarity
#define SPI_NCPHA	(1u<<1)	//Clock Phase
#define SPI_CSNAAT	(1u<<2)	//Chip Select Not Active After Transfer
#define SPI_CSAAT	(1u<<3)	//Chip Select Active After Transfer

/** SPI Modes*/
#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

/** Data Direction*/
#define MSBFIRST (1u)
#define LSBFIRST (0u)

/** SPI Clock Frequencies*/
#define SPI_CLOCK_DIV0			0x00000000	
#define SPI_CLOCK_DIV2			0x00000200	
#define SPI_CLOCK_DIV4			0x00000400	
#define SPI_CLOCK_DIV8			0x00000800
#define SPI_CLOCK_DIV16			0x00001000
#define SPI_CLOCK_DIV32			0x00002000	
#define SPI_CLOCK_DIV64			0x00004000	
#define SPI_CLOCK_DIV128		0x00008000	
#define SPI_CLOCK_DIV255		0x0000FF00	
#define SPI_CLOCK_FREQ(baud)	((uint32_t)(mstrClock/baud)<<8) //Variable Frequency, baud in Hz

/** SPI Number of Bits to Send*/
#define SPI_8BIT	0x00000000
#define SPI_9BIT	0x00000010
#define SPI_10BIT	0x00000020
#define SPI_11BIT	0x00000030
#define SPI_12BIT	0x00000040
#define SPI_13BIT	0x00000050
#define SPI_14BIT	0x00000060
#define SPI_15BIT	0x00000070
#define SPI_16BIT	0x00000080

/** Chip Select Options*/
#define SPI_CS0	0	//Digital Pin 10
#define SPI_CS1	1	//Digital Pin 4
#define SPI_CS2	2	//Digital Pin 52
#define SPI_CS3	3	//Not Connected on UDOO

/*------------------------------------------------------------------------------ */

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/


extern void SPI_Enable( Spi* spi ) ;
extern void SPI_Disable( Spi* spi ) ;
extern void SPI_EnableIt( Spi* spi, uint32_t dwSources ) ;
extern void SPI_DisableIt( Spi* spi, uint32_t dwSources ) ;

extern void SPI_Configure( Spi* spi, uint32_t id, uint32_t configuration ) ;
extern void SPI_ConfigureNPCS( Spi* spi, uint32_t npcs, uint32_t configuration ) ;

extern uint32_t SPI_Read( Spi* spi ) ;
extern void SPI_Write( Spi* spi, uint32_t npcs, uint16_t data ) ;

extern uint32_t SPI_GetStatus( Spi* spi ) ;
extern uint32_t SPI_IsFinished( Spi* pSpi ) ;


class SPIClass{
//Variables
	public:
	private:
	Spi *spi;
	uint32_t id;
	uint32_t interruptMode;
	uint32_t interruptSave;
	bool initialized;
	
	/** Settings for each CS pin (channel). 
	For example, if using the default chip select (digital pin 10), you can create SPI settings for the particular
	SPI device attached to that pin. The same goes for the other 3 chip selects.*/
	uint32_t bitOrder[NUM_SPI_CHANNELS];				//Specifies which order the bits are transmitted in, LSBFIRST or MSBFIRST
	uint32_t clockDivider[NUM_SPI_CHANNELS];			//Clock divider for channel
	uint32_t clockFrequency[NUM_SPI_CHANNELS];			//Clock frequency for channel
	uint32_t spiMode[NUM_SPI_CHANNELS];					//SPI mode (0-3) for channel
	uint32_t interruptMask[NUM_SPI_CHANNELS];			//Interrupt mask for channel
	uint32_t delayBeforeTransfer[NUM_SPI_CHANNELS];		//Delays the start of SPI transfer by x nanoseconds after chip select enable
	uint32_t delayAfterTransfer[NUM_SPI_CHANNELS];		//Delays disable of chip select for x nanoSeconds after successful SPI transfer
	uint32_t messageLength[NUM_SPI_CHANNELS];			//How many bits long is the data to send?
	uint16_t readBuff[NUM_SPI_CHANNELS];				//Stores the last recorded value from a read for each channel
	

	
//Functions
	public:
	void begin(uint8_t numChannels);
	void end();
	void setBitOrder(uint32_t pin, uint32_t bitOrder);
	void setClockDivider(uint32_t pin, uint32_t clockDiv);
	void setClockFrequency(uint32_t pin, uint32_t clockFreq);
	void setDataMode(uint32_t pin, uint32_t mode);
	void setInterruptMask(uint32_t pin, uint32_t mask);
	void setDelayAfterTrans(uint32_t pin, uint32_t delayVal);
	void setDelayBeforeTrans(uint32_t pin, uint32_t delayVal);
	void setMessageLength(uint32_t pin, uint32_t length);
	void updateSPISettings(uint32_t pin);
	void write(uint32_t pin, uint16_t data);
	uint32_t read();
	
	
	private:
	void init_SPI_PIO(uint8_t numChannels);
	void init_SPI(uint8_t numChannels);
	
};

#endif 

