//LoRa_Receiver_Logger_Settings.h
/*
******************************************************************************************************

LoRaTracker Programs for Arduino

Copyright of the author Stuart Robinson - 05/02/2018

http://www.LoRaTracker.uk
  
These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without
the explicit permission of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
intended purpose and free from errors.

To Do:


******************************************************************************************************
*/

//**************************************************************************************************
// 1) Hardware related definitions and options - specify board definition file type here
//**************************************************************************************************

#define UseSD                                     //to use SD card logging include this define

//#define UseDisplay                              //to use display include this define
const int Display_Address = 0x08;                 //for LoRaTracker display backpack when used

#define LoRa_Device_in_MB1                        //if using a LoRaTracker MikroBus based board specify the socket for the LoRa Device

/*
********************************************************************************************************************************
Hardware Connections

The program uses the hardware SPI interface on the Arduino to connect to the LoRa device, so the SPI SCK, MOSI and MISO pins are
assumed to be connected. The test program needs a minimum of one extra pin connected to act as chip select. Other pins may 
optionally be connected to the reset pin on the LoRa device, DIO0 to read RXReady and DIO2 so that the LoRa device can transmit
FM tones. You can explicitly define the required pins below by removing the two // characters in front of the #defines 

********************************************************************************************************************************
*/

#define lora_NSS 10                  //Arduino pin number for device select on LoRa device
#define lora_NReset 9                //Arduino pin number for reset pin on LoRa device, can be left not connected
#define lora_TonePin 7               //Arduino pin number connected to DIO2 pin on LoRa device, used for FM audio tone generation 
#define lora_DIO0 2                  //Arduino pin number connected to DIO0 pin on LoRa device, can be left not connected
#define LED1 8                       //Arduino pin number for LED, when high LED should be on.
#define BUZZ 5                       //Arduino pin number for buzzer, when high buzzer should be on.  
#define SD_CS 6                      //Arduino pin number for SD card select       


/*
***********************************************************************************************************************************************
As an alternative to explicitly defining the Arduino pins required, there are pre-defined board definition files for the LoRaTracker boards
included in the LoRaTracker Library;

https://github.com/LoRaTracker/LoRaTracker-Library

Select (include) the board definition file you require by removing the // characters before the appropriate include line in the list below

Note that for many of the boards below the use of an SD card for logging will not work, as some boards do not have the appropriate hardware
***********************************************************************************************************************************************
*/

//#include "HAB2_Board_Definitions.h"
//#include "Locator2_Board_Definitions.h"
//#include "LCD_Receiver_Board_Definitions.h"
//#include "Relay1_Board_Definitions.h"
//#include "Receiver2_Board_Definitions.h"
//#include "Sensor1_Board_Definitions.h"
//#include "MicroRX_Board_Definitions.h"
//#include "PIHTracker2_Board_Definitions.h"
//#include "PIHTracker3_Board_Definitions.h"
//#include "RFM98_Shield_January2016_Board_Definitions.h"
//#include "MarkTwo_Board_Definitions.h"
//#include "MicroBit_Board_Definitions.h"


//**************************************************************************************************
// 2) Program Options
//**************************************************************************************************

#define LOGASCII                                  //write to SD log as ASCII text
#define LOGHEX                                    //write to SD log as hex numbers in text

#define SERIALASCII                               //write to Serial monitor as ASCII text
#define SERIALHEX                                 //write to Serial monitor as hex numbers in text

//#define RAWONLY                                 //enable this define to ommit the time and packet info from Serial and SD

//#define CalibrateTone                           //comment in to have a calibrate tone at startup, can be used with a frequency counter to set calibration
//#define LORADEBUG                               //enable define to see LoRa debug messages


//**************************************************************************************************
// 3) Frequency settings
//**************************************************************************************************

const unsigned long Frequency = 434400000;     //set the LoRa frequency here  

const int CalibrationOffset = 0;               //this is the LoRa module frequency calibration offset in Hertz                  


//**************************************************************************************************
// 4) LoRa settings
//**************************************************************************************************


#define Bandwidth BW125000                     //for a fulllist of possible settings see the LoRa.h file
#define SpreadingFactor SF7
#define CodeRate CR45

const byte Deviation = 0x52;                   //typical deviation for FM tones
const int lora_RXBUFF_Size = 256;
const int lora_TXBUFF_Size = 1;

const byte Mode_Power = 10;                    //in dBm

#define USEDIO_RXReady                         //if you have access to the DIO0 pin on LoRa device enable this define

