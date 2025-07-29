
#include "DEV_Config.h"

/********************************************************************************
  function: System Init and exit
  note:
  Initialize the communication method
********************************************************************************/
SpiHandle spi_display;
GPIO pin_dc;
GPIO pin_reset;
/*------------------------------------------------------------------------------------------------------*/

void SPI_Config()
{
    SpiHandle::Config spi_config;
    // SPI peripheral config
    spi_config.periph = SpiHandle::Config::Peripheral::SPI_1;
    spi_config.mode   = SpiHandle::Config::Mode::MASTER;
    spi_config.direction
        = SpiHandle::Config::Direction::TWO_LINES_TX_ONLY;
    spi_config.datasize       = 8;
    spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
    spi_config.clock_phase    = SpiHandle::Config::ClockPhase::ONE_EDGE;
    spi_config.nss            = SpiHandle::Config::NSS::HARD_OUTPUT;
    spi_config.baud_prescaler = SpiHandle::Config::BaudPrescaler::PS_2;
    // SPI pin config
    spi_config.pin_config.sclk = Pin(PORTG, 11);
    spi_config.pin_config.miso = Pin(PORTB, 4);
    spi_config.pin_config.mosi = Pin(PORTB, 5);
    spi_config.pin_config.nss  = Pin(PORTG, 10);
    spi_display.Init(spi_config);
}

void SPI_Init()
{
    SPI_Config();
    // SSD1327 control pin config
    pin_dc.Init(Pin(PORTB, 6), GPIO::Mode::OUTPUT);
    pin_reset.Init(Pin(PORTB, 7), GPIO::Mode::OUTPUT);
}


/********************************************************************************
  function: Delay function
  note:
  Driver_Delay_ms(xms) : Delay x ms
  Driver_Delay_us(xus) : Delay x us
********************************************************************************/
void Driver_Delay_ms(unsigned long xms)
{
  System::Delay(xms);
}

void Driver_Delay_us(int xus)
{
  for (int j = xus; j > 0; j--);
}