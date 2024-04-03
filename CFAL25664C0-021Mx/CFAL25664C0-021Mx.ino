//==============================================================================
//
//  CRYSTALFONTZ 
//
//  This code drives the CFAL25664C0-021M-W/Y displays
//  https://www.crystalfontz.com/product/cfal25664C0021mw
//  https://www.crystalfontz.com/product/cfal25664C0021my
//
//  The controller is a Solomon Systech SSD1362
//    https://www.crystalfontz.com/controllers/SolomonSystech/SSD1362/
//
//  Seeeduino, an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//==============================================================================

//==============================================================================
// LCD & USD control lines
//   ARD      | Port  |             |  Function - 8080 Parallel   |  Function - SPI                     |  Function - I2C               
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  N/A	      |       | 2, 6, 8, 31 |  VCC -> POWER 12V           |  VCC -> POWER 12V                   |  VCC -> POWER 12V
//  3.3V      |       | 12, 15      |  VCI -> POWER 3.3V          |  VCI -> POWER 3.3V                  |  VCI -> POWER 3.3V
//  GND	      |       | 10-11       |  GROUND                     |  GROUND                             |  GROUND
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  N/A       | N/A   | 13          |  BS0 -> GND                 |  BS0 -> GND                         |  BS0 -> GND
//  N/A       | N/A   | 14          |  BS1 -> VCI                 |  BS1 -> GND                         |  BS1 -> VCI
//  N/A       | N/A   | 16          |  BS2 -> VCI                 |  BS2 -> GND                         |  BS2 -> GND
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  D8        | PORTB | 17          |  Chip Enable Signal  (CS)   |  Chip Enable Signal         (CS)    |  N/A pull low
//  D9        | PORTB | 18          |  Reset            (RESET)   |  Reset                      (RESET) |  Reset                    (RESET)
//  D10       | PORTC | 19          |  Data/Command        (DC)   |  Data/Command               (DC)    |  Slave Address Selection  (SAO)
//  A1        | PORTC | 20          |  Write               (WR)   |  N/A pull low                       |  Pull low (Sets I2C to write mode)    
//  A2        | PORTC | 21          |  Read                (RD)   |  N/A pull low                       |  N/A pull low      
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
// PARALLEL ONLY
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  D0        | PORTD | 22          |  LCD_D10 (DB0)              |                                     |
//  D1        | PORTD | 23          |  LCD_D11 (DB1)              |                                     |
//  D2        | PORTD | 24          |  LCD_D12 (DB2)              |                                     |
//  D3        | PORTD | 25          |  LCD_D13 (DB3)              |                                     |
//  D4        | PORTD | 26          |  LCD_D14 (DB4)              |                                     |
//  D5        | PORTD | 27          |  LCD_D15 (DB5)              |                                     |
//  D6        | PORTD | 28          |  LCD_D16 (DB6)              |                                     |
//  D7        | PORTD | 29          |  LCD_D17 (DB7)              |                                     |
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
// SPI ONLY
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  D13       | PORTD | 22          |                             |  SCLK                               |
//  D11       | PORTD | 23          |                             |  SDIN                               |
//  D2        | PORTD | 24          |                             |  No Connection                      |
//  D3        | PORTD | 25          |                             |  N/A pull low                       |
//  D4        | PORTD | 26          |                             |  N/A pull low                       |
//  D5        | PORTD | 27          |                             |  N/A pull low                       |
//  D6        | PORTD | 28          |                             |  N/A pull low                       |
//  D7        | PORTD | 29          |                             |  N/A pull low                       |
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
// I2C ONLY
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------
//  A5       | PORTC  | 22          |                             |                                     | SCLK 
//  A6       | PORTC  | 23, 24      |                             |                                     | SDA      
//------------+-------+-------------+-----------------------------+------------------------------------------------------------------------

// Consult the datasheet for more interface options
//==============================================================================
//
// There are additional components that should be soldered prior to interfacing.
// Please consult the datasheet for these components.
//
//==============================================================================
//  BS0,BS1 interface settings:
//  
//      Interface         | BS0 | BS1 | BS2 
//  ----------------------+-----+-----+-----
//    4-wire SPI          |  0  |  0  |  0  
//    3-wire SPI          |  1  |  0  |  0  
//    8-bit 8080 Parallel |  0  |  1  |  1  
//    8-bit 6800 Parallel |  0  |  0  |  1  
//    I2C                 |  0  |  1  |  0  
//
//  This code is demonstrated using 8080 Parallel, 4-wire SPI, and I2C
//==============================================================================
//To use SD:
#define SD_CS 10
//  ARD       | SD  
// -----------+-----
//  SD_CS     | CS    
//  D11	      | MOSI     
//  D12       | MISO     
//  D13       | CLK     
//==============================================================================
//  Define the interface
//#define SPI_4_WIRE
//#define PAR_8080
#define I2C
//==============================================================================

#define CLR_CS     (PORTB &= ~(0x01)) //pin #8  - Chip Enable Signal
#define SET_CS     (PORTB |=  (0x01)) //pin #8  - Chip Enable Signal
#define CLR_RESET  (PORTB &= ~(0x02)) //pin #9 - Reset
#define SET_RESET  (PORTB |=  (0x02)) //pin #9 - Reset
#define CLR_DC     (PORTB &= ~(0x04)) //pin #10 - Data/Instruction
#define SET_DC     (PORTB |=  (0x04)) //pin #10 - Data/Instruction
#define CLR_WR	   (PORTC &= ~(0x02)) //pin #15 - Write
#define SET_WR	   (PORTC |=  (0x02)) //pin #15 - Write
#define CLR_RD	   (PORTC &= ~(0x04)) //pin #16 - Read
#define SET_RD	   (PORTC |=  (0x04)) //pin #16 - Read

#include <SD.h>
#include <avr/io.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "splash.h"

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define HRES      128
#define VRES      56
#define	Brightness	0xBF

SoftwareSerial mySerial(19, 18);

#ifdef SPI_4_WIRE
//================================================================================
void writeCommand(uint8_t command)
{
  // Select the LCD controller
  CLR_CS;
  // Select the LCD's command register
  CLR_DC;

  //Send the command via SPI:
  SPI.transfer(command);
  //deselect the controller
  SET_CS;

}
//================================================================================
void writeData(uint8_t data)
{
  //Select the LCD's data register
  SET_DC;
  //Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(data);

  // Deselect the LCD controller
  SET_CS;
}
#endif
//================================================================================

#ifdef PAR_8080
//================================================================================
void writeCommand(uint8_t command)
{
  //  delay(10);
  CLR_CS;
  CLR_DC;

  PORTD = command;
  CLR_WR;
  SET_WR;
  SET_CS;
}

//================================================================================
void writeData(uint8_t data)
{
  //select the LCD controller
  CLR_CS;
  //select the LCD's data register
  SET_DC;
  //send the data via parallel
  PORTD = data;
  //clear the write register
  CLR_WR;
  //set the write register
  SET_WR;
  //deselct the LCD controller
  SET_CS;
}

//================================================================================
void writeDataFast(uint8_t data)
{
  //send the data via parallel
  PORTD = data;
  //clear the write register
  CLR_WR;
  //set the write register
  SET_WR;
}

//================================================================================
uint8_t readData()
{
  CLR_CS;
  SET_DC;
  SET_RD;
  SET_WR;
  uint8_t response = 0x00;
  PORTD = 0x00;
  //set pins to input
  DDRD = 0x00;
  CLR_RD;
  SET_RD;
  response = PIND;
  //Set pins back to output
  DDRD = 0xFF;
  SET_CS;
  return response;
}
#endif
//================================================================================

#ifdef I2C
//================================================================================
void writeCommand(uint8_t command)
{
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);
  Wire.write(command);
  Wire.endTransmission();
}

//================================================================================
void writeData(uint8_t data)
{
  Wire.beginTransmission(0x3C);
  Wire.write(0x40);
  Wire.write(data);
  Wire.endTransmission();
}
#endif
//================================================================================

void setColAddr(uint8_t start, uint8_t end)
{
  writeCommand(0x15);
  writeCommand(start);		    // Set start column address
  writeCommand(end);			    // Set end column address
}

void setRowAddr(uint8_t start, uint8_t end)
{
  writeCommand(0x75);
  writeCommand(start);		    // Set start row address
  writeCommand(end);			    // Set end row address
}

void setStartLine(uint8_t line)
{
  writeCommand(0x40 | line);	// Set  Display start line
}

void setContrast(uint8_t contrast)
{
  writeCommand(0x81);			    // Set contrast control
  writeCommand(contrast);	    //   Default => 0x80 (Maximum)
}

//================================================================================
void clearScreen()
{
  unsigned char i, j;

  for (i = 0; i < 64; i++)
  {
    setColAddr(0x00, 0x7f);	   // Set higher & lower column Address
    setRowAddr(i, 0x3f);				   //  Set Row-Address 

    for (j = 0; j < 128; j++)
    {
      writeData(0x00);
    }
  }
}

//================================================================================
void displayFromFlash()
{
  unsigned char i, j;

  for (i = 0; i < 64; i++)
  {
    setColAddr(0x00,0x7f);	   // Set higher & lower column Address
    setRowAddr(i, 0x3f);				   //  Set Row-Address 

    for (j = 0; j < 128; j++)
    {
      //wrap the address in a pgm_read so we can get to the data
      writeData(pgm_read_byte(&Grey_2BPP[i * 128 + j]));
    }
  }
}

//=================================================================================
void showGradient()
{
  for (uint8_t i = 0; i < 64; i++)
  {
    setColAddr(0x00, 0x7f);	   // Set higher & lower column Address
    setRowAddr(i, 0x3f);				   //  Set Row-Address 

    uint8_t gradient = 0x00;
    for (uint8_t j = 0; j < 128; j++)
    {
      writeData(gradient);
      if (j % 16 == 0)
      {

        if ((gradient & 0x01) == 0x00)
        {
          gradient |= 0x11;
        }
        else
        {
          gradient = gradient << 1;
        }
      }
    }
  }
}

//================================================================================
void displayInit()				
{
  unsigned int i;

  CLR_RESET;
  delay(100);
  SET_RESET;

  writeCommand(0Xfd); //Set Command Lock
  writeCommand(0X12); //(12H=Unlock,16H=Lock)

  writeCommand(0XAE); //Display OFF(Sleep Mode)

  setColAddr(0x00, 0x7f);

  setRowAddr(0x00, 0x3f);

  writeCommand(0X81); //Set contrast
  writeCommand(0x2f);

  writeCommand(0Xa0); //Set Remap
  writeCommand(0Xc3);

  writeCommand(0Xa1); //Set Display Start Line
  writeCommand(0X00);

  writeCommand(0Xa2); //Set Display Offset
  writeCommand(0X00);

  writeCommand(0Xa4); //Normal Display

  writeCommand(0Xa8); //Set Multiplex Ratio
  writeCommand(0X3f);

  writeCommand(0Xab); //Set VDD regulator
  writeCommand(0X01); //Regulator Enable

  writeCommand(0Xad); //External /Internal IREF Selection
  writeCommand(0X8E);

  writeCommand(0Xb1); //Set Phase Length
  writeCommand(0X22);

  writeCommand(0Xb3); //Display clock Divider
  writeCommand(0Xa0);

  writeCommand(0Xb6); //Set Second precharge Period
  writeCommand(0X04);

  writeCommand(0Xb9); //Set Linear LUT

  writeCommand(0Xbc); //Set pre-charge voltage level
  writeCommand(0X10); //0.5*Vcc

  writeCommand(0Xbd); //Pre-charge voltage capacitor Selection
  writeCommand(0X01);

  writeCommand(0Xbe); //Set cOM deselect voltage level
  writeCommand(0X07); //0.82*Vcc

  clearScreen();				// Clear Screen
  writeCommand(0Xaf); //Display ON
}

//================================================================================
void setup()
{
  //setup ports
  DDRD = 0xff;
  DDRC = 0xff;
  DDRB = 0x07;

  PORTD = 0xff;

  SET_RD;
  SET_WR;
  SET_CS;
  CLR_DC;

//  Serial.begin(9600);
//  delay(100);
//  for (int i = 0; i < 20; i++)
//    mySerial.println();
//  mySerial.println("Serial Initialized");
//  //If an SD card is connected, do the SD setup
//  if (!SD.begin(SD_CS))
//  {
//    mySerial.println("SD could not initialize");
//  }
#ifdef SPI_4_WIRE
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
#endif

#ifdef I2C
  Wire.begin();
#endif
  //initialize the display
  displayInit();
}

//================================================================================
#define waittime      5000
#define gradient_demo 1
#define splash_demo   1
void loop()
{
#if gradient_demo
   showGradient();
   delay(waittime);
#endif

#if splash_demo
   displayFromFlash();
   delay(waittime);
#endif
while(1);
}
