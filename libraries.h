/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _LIBRARIES_
#define _LIBRARIES_

/*
 * Core and peripherals registers definitions
 */
#include "sam.h"
#include "include/variant.h"

//Dependencies
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "include/gccPatch.h"

/************************************************************************/
/* Most of these libraries have been customized. Use with caution.      */
/************************************************************************/
#include "include/can.h"
#include "include/dacc.h"
#include "include/delay.h"
#include "include/ioController.h"
#include "include/pio.h"
#include "include/pmc.h"
#include "include/pwmc.h"
#include "include/ringbuffer.h"
#include "include/rtc.h"
#include "include/rtt.h"
#include "include/spi.h"
#include "include/tc.h"
#include "include/timetick.h"
#include "include/twi.h"
#include "include/UART.h"
#include "include/USART.h"

/************************************************************************/
/* Peripheral Libraries                                                 */
/************************************************************************/
#include "Peripherals/LSM9DS0.h"
#include "Peripherals/NeoPixel.h"


/************************************************************************/
/* Global Definitions                                                   */
/************************************************************************/
#define NONE (0u)
#define MASTER_CLOCK 84000000L


/************************************************************************/
/* Class Objects                                                        */
/************************************************************************/

/*
I2C
*/
extern TwoWireClass Wire;
extern TwoWireClass Wire1;

/*
Serial
*/
extern UARTClass  Serial;
extern USARTClass Serial1;
extern USARTClass Serial2;
extern USARTClass Serial3;

/*
SPI
*/
extern SPIClass SPI;

#endif /* _LIBRARIES_ */
