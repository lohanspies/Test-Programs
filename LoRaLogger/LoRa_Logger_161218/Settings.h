//Settings.h for logger
/*
******************************************************************************************************

LoRaTracker Programs for Arduino

Copyright of the author Stuart Robinson

http://www.LoRaTracker.uk

These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without
the explicit permission of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
intended purpose and free from errors.

To Do:

******************************************************************************************************
*/

//*******  Setup LoRa Parameters Here ! ***************

//LoRa Modem Parameters
const uint32_t Frequency = 434400000;               //frequency of transmissions
const uint16_t CalibrationOffset = 0;               //adjustment for frequency in Hz, assumed at room temp

#define Bandwidth BW125000                          //LoRa bandwidth
#define SpreadingFactor SF7                         //LoRa spreading factor
#define CodeRate CR45                               //LoRa coding rate

//#define UseLoRaWAN                                //enable define for LoRaWAN modem parameters
//#define UseSX1272                                 //enable define to use SX1272    

//#define LORADEBUG                               //if enabled there are extra debug messages to serial monitor

#define EnableTone                                //Enable ident tone at startup, do not enable if using LNA on receiver

//#define lora_RXBUFF_Size 128
//#define lora_TXBUFF_Size 1

//*******  Set Logging options Here ! ***************

#define SERIALASCII                             //write to Serial monitor as ASCII text
#define SERIALHEX                                 //write to Serial monitor as hex numbers in text

#define USESD                                   //to use SD card logging include this define
#define LOGASCII                                //write to SD log as ASCII text
#define LOGHEX                                    //write to SD log as hex numbers in text



//*******  Set Display options Here ! ***************

#define UseDisplay                               //to use a display, enable this define 
#define UseI2CLCD                                 //to use a 20x4 LCD with a PCF8574 backpack, enable this define
//#define UseSSD1306                              //to use an SSD1306 I2C display, enable this define                             

#define SSD1306_Address 0x3C                      //define I2C address for SSD1306
#define LCDI2C_Address 0x3F                       //define I2C address for PCF8574 LCD backpack





