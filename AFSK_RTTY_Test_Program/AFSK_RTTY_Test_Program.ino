#define programname "AFSK_RTTY_Test_Program"
#define programversion "V1.2"
#define dateproduced "05/12/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#include "Program_Definitions.h"

/*
*****************************************************************************************************************************

LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 05/12/2017

http://www.LoRaTracker.uk

These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
free from errors.

*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Program operation

This test program has been written to check that hardware for sending AFSK RTTY on the receiver board has been connected
correctly. The audio output is connected to a PC sound cards microphone input. The AFSK RTTY is snet as ASCII 7 bit, 2 Stop bit,
no parity, 300 baud. Tones are 634Hz for a 0 bit and 1000hz for 1 bit. A screenshot of the FLDIGI settings used in in the folder
containing the program; 'FLDIGI Settings.jpg'

********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses 2 pins on the Arduino, Audio_Out and LED1. You can explicitly define them below by removing the two // characters
in front of the #define Audio_Out and #define LED1 lines.
********************************************************************************************************************************
*/

#define Audio_Out 6              //Pin used to output Audio tones  
#define LED1 10                  //LED1 is toggled inside the AFSK_RTTY library, high for logic 1, low for logic 0, so it can be used to check the timing.

/*
***********************************************************************************************************************************************
Board Definitions

As an alternative to explicitly defining the Arduino pins required, there are pre-defined board definition files for the LoRaTracker boards
included in the LoRaTracker Library;

https://github.com/LoRaTracker/LoRaTracker-Library

Select (include) the board definition file you require by removing the // characters before the appropriate include line in the list below
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

#define Serial_Monitor_Baud 38400        //this is baud rate used for the Arduino IDE Serial Monitor

const int AFSKrttybaud = 1470;           //delay in uS x 2 for 1 bit. 4700 = 100baud, 2295 = 200baud, 1470 = 300baud 
const int afskleadinmS = 500;            //number of ms for AFSK constant lead in tone
const int tonehighHz = 1000;             //high tone in Hertz 
const int tonelowHz = 634;               //low tone in Hertz   

#include "AFSK_RTTY.h"

String TestString = "www.LoRaTracker.uk - Hello World";               //This string is sent as AFSK RTTY, 7 bit, 2 Stop bit, no parity, 300 baud.

byte TXBUFF[64];
byte Output_len_max = 64;
byte Count;
unsigned int Loopcount = 0;


void loop()
{
  byte index, j;
  Serial.print(F("Send AFSK RTTY "));
  Loopcount++;
  Count = build_message();

  start_AFSK_RTTY();
  
  for (index = 0; index <= Count; index++)
  {
    j = TXBUFF[index];
    SendAFSKRTTY(j);
  }

  SendAFSKRTTY(13);

  Serial.println();

  delay(1500);

}


byte build_message()
{
  memset(TXBUFF, 0, sizeof(TXBUFF));                     //clear array

  Count = snprintf((char*) TXBUFF,
           Output_len_max,
           "www.LoRaTracker.uk - Hello World %d",
           Loopcount
           );

  return Count;
}


void setup()
{
  pinMode(LED1, OUTPUT);                                 //for PCB LED
  Serial.begin(Serial_Monitor_Baud);                                   //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

}


