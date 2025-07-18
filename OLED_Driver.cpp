/*****************************************************************************
* | File      	:   OLED_Driver.cpp
* | Author      :   Waveshare team
* | Function    :   1.5inch OLED Module Drive function
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2020-08-19
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "OLED_Driver.h"
#include "stdio.h"
#include "core_cm7.h"

static volatile bool dmaReady = true;

/*******************************************************************************
function:
            Hardware reset
*******************************************************************************/
static void OLED_Reset(void)
{
    pin_reset.Write(true);
    Driver_Delay_ms(10);
    pin_reset.Write(false);
    Driver_Delay_ms(10);
    pin_reset.Write(true);
    Driver_Delay_ms(10);
}

/*******************************************************************************
function:
            Write register address and data
*******************************************************************************/
static void OLED_WriteReg(uint8_t Reg)
{
    pin_dc.Write(false);
    spi_display.BlockingTransmit(&Reg, 1);
}

static void OLED_WriteData(uint8_t Data)
{
    pin_dc.Write(true);
    spi_display.BlockingTransmit(&Data, 1);
}

/*******************************************************************************
function:
        Common register initialization
*******************************************************************************/
static void OLED_InitReg(void)
{
    OLED_WriteReg(0xae);//--turn off oled panel

    OLED_WriteReg(0x15);    //   set column address
    OLED_WriteReg(0x00);    //  start column   0
    OLED_WriteReg(0x7f);    //  end column   127

    OLED_WriteReg(0x75);    //   set row address
    OLED_WriteReg(0x00);    //  start row   0
    OLED_WriteReg(0x7f);    //  end row   127

    OLED_WriteReg(0x81);  // set contrast control
    OLED_WriteReg(0x80);

    OLED_WriteReg(0xa0);    // gment remap
    OLED_WriteReg(0x51);   //51

    OLED_WriteReg(0xa1);  // start line
    OLED_WriteReg(0x00);

    OLED_WriteReg(0xa2);  // display offset
    OLED_WriteReg(0x00);

    OLED_WriteReg(0xa4);    // rmal display
    OLED_WriteReg(0xa8);    // set multiplex ratio
    OLED_WriteReg(0x7f);

    OLED_WriteReg(0xb1);  // set phase leghth
    OLED_WriteReg(0xf1);

    OLED_WriteReg(0xb3);  // set dclk
    OLED_WriteReg(0x00);  //80Hz:0xc1 90Hz:0xe1   100Hz:0x00   110Hz:0x30 120Hz:0x50   130Hz:0x70     01

    OLED_WriteReg(0xab);  //
    OLED_WriteReg(0x01);  //

    OLED_WriteReg(0xb6);  // set phase leghth
    OLED_WriteReg(0x0f);

    OLED_WriteReg(0xbe);
    OLED_WriteReg(0x0f);

    OLED_WriteReg(0xbc);
    OLED_WriteReg(0x08);

    OLED_WriteReg(0xd5);
    OLED_WriteReg(0x62);

    OLED_WriteReg(0xfd);
    OLED_WriteReg(0x12);

}

/********************************************************************************
function:
            initialization
********************************************************************************/
void OLED_1in5_Init()
{
    SPI_Init();
    // Hardware reset
    OLED_Reset();
    // Set the initialization register

    OLED_InitReg();
    Driver_Delay_ms(200);

    // Turn on the OLED display
    OLED_WriteReg(0xaf);
}
/********************************************************************************
function:   Set the display Window(Xstart, Ystart, Xend, Yend)
parameter:
        xStart :   X direction Start coordinates
        Ystart :   Y direction Start coordinates
        Xend   :   X direction end coordinates
        Yend   :   Y direction end coordinates
********************************************************************************/
static void OLED_SetWindow(UBYTE Xstart, UBYTE Ystart, UBYTE Xend, UBYTE Yend)
{
    if((Xstart > OLED_1in5_WIDTH) || (Ystart > OLED_1in5_HEIGHT) ||
       (Xend > OLED_1in5_WIDTH) || (Yend > OLED_1in5_HEIGHT))
        return;

    OLED_WriteReg(0x15);
    OLED_WriteReg(Xstart/2);
    OLED_WriteReg(Xend/2 - 1);

    OLED_WriteReg(0x75);
    OLED_WriteReg(Ystart);
    OLED_WriteReg(Yend - 1);
}

/********************************************************************************
function:
            Clear screen
********************************************************************************/
void OLED_1in5_Clear(void)
{
    UWORD i;
    OLED_SetWindow(0, 0, 128, 128);
    for(i=0; i<OLED_1in5_WIDTH*OLED_1in5_HEIGHT/2; i++){
        OLED_WriteData(0x00);
    }
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_Display(const UBYTE *Image)
{
    UWORD i, j, temp;
    OLED_SetWindow(0, 0, 128, 128);
    for(i=0; i<OLED_1in5_HEIGHT; i++)
        for(j=0; j<OLED_1in5_WIDTH/2; j++)
        {
            temp = Image[j + i*64];
            OLED_WriteData(temp);
        }
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_Display_Part(const UBYTE *Image, UBYTE Xstart, UBYTE Ystart, UBYTE Xend, UBYTE Yend)
{
    UWORD i, j, temp;
    OLED_SetWindow(Xstart, Ystart, Xend, Yend);
    for(i=0; i<Yend-Ystart; i++)
        for(j=0; j<(Xend-Xstart)/2; j++)
        {
            temp = Image[j + i*(Xend-Xstart)/2];
            OLED_WriteData(temp);
        }
}
//--------------------------------------------------------
// DMA ready callback functions
//--------------------------------------------------------
void OLED_Dma_Ready(void*, daisy::SpiHandle::Result result)
{
    dmaReady = (result == daisy::SpiHandle::Result::OK);
}

void OLED_Transmit_DMA(ImageData *Imagedata)
{
    if (dmaReady)
    {
        OLED_SetWindow(0, 0, 128, 128);
        pin_dc.Write(true);
        dmaReady = false;
        SCB_CleanDCache_by_Addr((uint32_t*)Imagedata->data, Imagedata->size);
        spi_display.DmaTransmit((UBYTE*)Imagedata->data, Imagedata->size, NULL, OLED_Dma_Ready, NULL);
    }
}

void OLED_Part_Transmit_DMA(ImageData *Imagedata, UBYTE Xstart, UBYTE Ystart, UBYTE Xend, UBYTE Yend)
{
    while (!dmaReady) {
        System::Delay(1); 
    }
    
    dmaReady = false;
    
    OLED_SetWindow(Xstart, Ystart, Xend, Yend);
    pin_dc.Write(true);
    
    uint32_t dmaSize = ((Xend - Xstart + 1) / 2) * (Yend - Ystart + 1);
    
    SCB_CleanDCache_by_Addr((uint32_t*)Imagedata->data, dmaSize);    
    spi_display.DmaTransmit((UBYTE*)Imagedata->data, dmaSize, NULL, OLED_Dma_Ready, NULL);
}