/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#include "as32m0_gpio.h"

#include <stdint.h>  // for uint8_t etc
#include <stdbool.h> // for bool
#include <stddef.h>  // for NULL
#include <string.h>  // for memset()
#include "LedControl.h"
#include "max7219_config.h"

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15
// maxinum digits of MAX7219 matrix display
#define MAX_MATRIX_DIGIT 16



		/* The array for shifting the data to the devices */
		static uint8_t spidata[16];
		/* We keep track of the led-status for all 8 devices in this array */
		static uint8_t status[64];
		/* Data is shifted out of this pin*/
		static uint32_t SPI_MOSI;
		/* The clock is signaled on this pin */
		static uint32_t SPI_CLK;
		/* This one is driven LOW for chip selectzion */
		static uint32_t SPI_CS;
		/* The maximum number of devices we use */
		static uint8_t maxDevices;


void ledControl_init(uint32_t dataPin, uint32_t clkPin, uint32_t csPin, uint8_t numDevices) {
		//printf("logProc ledControl init\n");
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    if(numDevices<=0 || numDevices>MAX_MATRIX_DIGIT )
        numDevices=MAX_MATRIX_DIGIT;
    maxDevices=numDevices;
		//printf("numDevices=%d\n", numDevices);
    //digitalWrite(SPI_CS,HIGH);
		ioWrite.SET = SPI_CS;
    SPI_MOSI=dataPin;
    for(int i=0;i<64;i++) 
        status[i]=0x00;
    for(int i=0;i<maxDevices;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        clearDisplay(i);
        //we go into shutdown-mode on startup
        shutdown(i,false);
    }
}

uint8_t getDeviceCount() {
    return maxDevices;
}

void shutdown(int addr, bool b) {
		//printf("LED control shutdown\n");
    if(addr<0 || addr>=maxDevices)
        return;
    if(b)
        spiTransfer(addr, OP_SHUTDOWN,0);
    else
        spiTransfer(addr, OP_SHUTDOWN,1);
}

void setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT,limit);
}

void setIntensity(int addr, int intensity) {
		//printf("set intensity\n");
    if(addr<0 || addr>=maxDevices)
        return;
    if(intensity>=0 && intensity<16)	
        spiTransfer(addr, OP_INTENSITY,intensity);
}

void clearDisplay(int addr) {
    uint8_t offset;

		//printf("clear display #%d\n", addr);
    if(addr<0 || addr>=maxDevices)
        return;
    offset=addr*8;
    for(int i=0;i<8;i++) {
        status[offset+i]=0;
        spiTransfer(addr, i+1,status[offset+i]);
    }
}

void setLed(int addr, int row, int column, bool state) {
    int offset;
    uint8_t val=0x00;

    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=addr*8;
    val=0x80 >> column;
    if(state)
        status[offset+row]=status[offset+row]|val;
    else {
        val=~val;
        status[offset+row]=status[offset+row]&val;
    }
    spiTransfer(addr, row+1, status[offset+row]);
}

void setRow(int addr, int row, uint8_t value) {
    int offset;
		//printf("set row %d addr 0x%04X, offset %d device=%d\n", row, addr, offset, maxDevices);
    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7)
        return;
		//printf("here we go\n");
    offset=addr*8;
    status[offset+row]=value;
		
    spiTransfer(addr, row+1, status[offset+row]);
}

//void setColumn(int addr, int col, uint8_t value) {
//    uint8_t val;

//		printf("set column\n");
//    if(addr<0 || addr>=maxDevices)
//        return;
//    if(col<0 || col>7) 
//        return;
//		
//    for(int row=0;row<8;row++) {
//        val=value >> (7-row);
//        val=val & 0x01;
//        setLed(addr,row,col,val);
//    }
//}

void spiTransfer(uint8_t addr, volatile uint8_t opcode, volatile uint8_t data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;
		//printf("MaxDevice %d, max bytes %d\n", maxDevices, maxbytes);
		//printf("SPI transfer %d x%02X x%02X\n", addr, opcode, data);
    for(int i=0;i<maxbytes;i++)
        spidata[i]=(uint8_t)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    //digitalWrite(SPI_CS,LOW);
		ioWrite.CLR = SPI_CS;
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--){
#ifdef __SOFTSPI			
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
//				printf("shiftout 0x%04X\n", spidata[i-1]);
#else
	#ifndef __SOFTSPI
				APB_SPI->DataRegister = spidata[i-1];
				apSSP_DeviceEnable(APB_SPI);
				while(apSSP_DeviceBusyGet(APB_SPI));
				apSSP_DeviceDisable(APB_SPI);
				apSSP_ReadFIFO(APB_SPI);
			
	#endif
#endif			
		}
    //latch the data onto the display
    //digitalWrite(SPI_CS,HIGH);
		ioWrite.SET = SPI_CS;
}    

#ifdef __SOFTSPI
void shiftOut(uint32_t dataPin, uint32_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST)
            //digitalWrite(dataPin, (val & (1 << i)));
					if((val & (1 << i)) == 1<<i) ioWrite.SET = dataPin;  
					else ioWrite.CLR = dataPin;
        else if (bitOrder == MSBFIRST){   
            //digitalWrite(dataPin, (val & (1 << (7 - i))));
					if((val & (1 << (7 - i))) == (1<<(7-i))) ioWrite.SET = dataPin;
					else ioWrite.CLR = dataPin;
				}

        //digitalWrite(clockPin, HIGH);
        //digitalWrite(clockPin, LOW);    
				ioWrite.SET = clockPin;
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				__NOP();
				ioWrite.CLR = clockPin;
    }
}

#endif


