/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	DEV_Config.h
* |	version		  :	V1.0
* | date		    :	2020-06-16
* | function	  :	Provide the hardware underlying interface	
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include "daisy.h"
#include "per/gpio.h"
#include "per/spi.h"


using namespace daisy;

extern SpiHandle spi_display;
extern GPIO pin_dc;
extern GPIO pin_reset;

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/****************************************************************************************
    //Use the  library function definition
*****************************************************************************************/
//GPIO config

void SPI_Config();
void SPI_Init();

void Driver_Delay_ms(unsigned long xms);
void Driver_Delay_us(int xus);

#endif
