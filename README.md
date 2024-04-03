# CFAL25664C0-021Mx Demonstration Code

This is Arduino sample code for the CFAL25664C0-021MW/Y family of displays. These displays are graphic OLEDs and can be interfaced with using 3- or 4-wire SPI, I2C, 6800, or 8080 parallel and use the [Solomon Systech SSD1362](https://www.crystalfontz.com/controllers/Solomon%20Systech/SSD1362/490) display controller. For interfacing and bringing up of the displays/prototyping, we recommend using the [CFA10102 Multi-use ZIF Breakout Board](https://www.crystalfontz.com/product/cfa10102-oled-breakout-board) which includes a voltage booster making it ideal for OLEDs which require a high panel voltage. In addition to the connection guide outlined below, please refer to the displays datasheet which outlines additional components required for the bring-up of the displays.

## Connection Guide
```
/   ARD      | Port  |             |  Function - 8080 Parallel   |  Function - SPI                     |  Function - I2C               
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
```

## Display Information
Here are links to our active displays:\
[CFAL25664C0-021M-W](https://www.crystalfontz.com/product/cfal25664c0021mw-256x64-graphic-oled)\
[CFAL25664C0-021M-Y](https://www.crystalfontz.com/product/cfal25664c0021my-256x64-yellow-monochrome-oled-display)

For more inforamtion about other OLED offerings, please see our full list [here](https://www.crystalfontz.com/c/oled-displays/29).