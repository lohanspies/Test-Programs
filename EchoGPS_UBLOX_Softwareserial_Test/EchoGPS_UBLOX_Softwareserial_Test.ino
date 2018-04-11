#define programname "EchoGPS_UBLOX_Softwareserial_Test"
#define programversion "V1.1"
#define dateproduced "28/11/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#include "Program_Definitions.h"                     //part of LoRaTracker library
#define GPS_in_MB2                                   //required if board is using Mikrobus sockets

/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 28/11/2017

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

The purpose of this program is to check that a Serial UBLOX GPS is working. First a series of configuration commands are sent 
to the GPS and the acknowlegements checked. The GPS is put into navigation model 6 (for 18,000m + operation) and the navigation
model is polled and checked. Characters are then read from the GPS at 9600 baud (default) using software serial and sent to the
Arduino IDE Serial Monitor at 115200 baud (default).
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses two pins for the software serial interface on the Arduino. You can explicitly define the required pins below
by removing the // characters in front of the 3 #defines 
********************************************************************************************************************************
*/

//#define LED1 10                            //pin for LED  
//#define GPSTX A2                           //pin number for TX output - Arduino into GPS
//#define GPSRX A3                           //pin number for RX input - To Arduino from GPS
//#define GPSPOWER -1                        //not used for non LoRaTracker boards, leave at -1

/*
***********************************************************************************************************************************************
Board Definitions

As an alternative to explicitly defining the Arduino pins required, there are pre-defined board definition files for the LoRaTracker boards
included in the LoRaTracker Library;

HTTPS://github.com/LoRaTracker/LoRaTracker-Library

Select (include) the board definition file you require by removing the // characters before the appropriate include line in the list below
***********************************************************************************************************************************************
*/

//#include "HAB2_Board_Definitions.h"
#include "Locator2_Board_Definitions.h"
//#include "LCD_Receiver_Board_Definitions.h"
//#include "Relay1_Board_Definitions.h"
//#include "Receiver2_Board_Definitions.h"
//#include "Sensor1_Board_Definitions.h"
//#include "MicroRX_Board_Definitions.h"
//#include "PIHTracker2_Board_Definitions.h"
//#include "PIHTracker3_Board_Definitions.h"
//#include "RFM98_Shield_January2016_Board_Definitions.h"
//#include "MarkTwo_Board_Definitions.h"


#define GPSBaud 9600                        //baud rate for GPS 
#define Serial_Monitor_Baud 115200          //this is baud rate used for the Arduino IDE Serial Monitor

#include <NeoSWSerial.h>                    //https://github.com/SlashDevin/NeoSWSerial  
NeoSWSerial GPSserial(GPSRX, GPSTX);        //this library is far more reliable at GPS init than standard softwareserial

const unsigned long GPS_WaitAck_mS = 2000;  //number of mS to wait for an ACK response from GPS
const byte GPS_attempts = 3;                //number of times the sending of GPS config will be attempted.
const byte GPS_Reply_Size = 12;             //size of GPS reply buffer
const unsigned int GPS_Clear_DelaymS = 2000;     //mS to wait after a GPS Clear command is sent
boolean GPS_Config_Error;  

#define GPS_ALLOW_GPGSV                     //we want to see the GPGSV messages 
#include "UBLOX_SerialGPS2.h"               //from LoRaTracker library 


void loop()                    
{
  while (GPSserial.available() > 0)
  Serial.write(GPSserial.read());
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}


void setup()
{
  pinMode(GPSPOWER, OUTPUT);	               //setup pin for GPS Power Control
  digitalWrite(GPSPOWER, LOW);
 
  Serial.begin(115200);                      //connect at 115200 so we can read the GPS fast enough and also spit it out
  Serial.println();
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));

  pinMode(LED1, OUTPUT);                     //for Watchdog pulse input
  led_Flash(2, 500);

  pinMode(GPSPOWER, OUTPUT);                 //in case power switching components are fitted
  GPS_On(DoGPSPowerSwitch);                 //this will power the GPSon
  GPS_Setup();

  if (!GPS_CheckNavigation())                //Check that UBLOX GPS is in Navigation model 6
  {
    Serial.println();
    Serial.println(F("Warning GPS Error !"));
    Serial.println();
    led_Flash(100, 25);
  }

  Serial.println(F("Starting GPS Read"));
}


