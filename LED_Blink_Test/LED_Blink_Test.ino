#define programname "LED_Blink_Test"
#define programversion "V1.0"
#define dateproduced "26/11/2017"
#define aurthorname "Stuart Robinson"
//#include <Arduino.h>

/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 26/11/2017

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

The program flashes a LED connected to the pin defined by LED1, the LED on pin 13 of most Arduinos is flashed as well.
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

You can explicitly define the required pin below by removing the two // characters in front of the #defines 
********************************************************************************************************************************
*/

#define LED1 8                      //Arduino pin number for LED, when high LED should be on.

/*
***********************************************************************************************************************************************
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
//#include "Receiver2_Board_Defintions.h"
//#include "Sensor1_Board_Definitions.h"
//#include "MicroRX_Board_Definitions.h"
//#include "PIHTracker2_Board_Definitions.h"
//#include "PIHTracker3_Board_Definitions.h"
//#include "RFM98_Shield_January2016_Board_Definitions.h"
//#include "MarkTwo_Board_Definitions.h"


#define Serial_Monitor_Baud 115200          //this is baud rate used for the Arduino IDE Serial Monitor

unsigned int seconds;

void loop() 
{
  Serial.print(seconds);
  Serial.println(F(" Seconds"));            //this message should print on console close to once per second   
  seconds++;
  led_Flash(4,125);
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to show tracker is alive
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(13, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    digitalWrite(13, LOW);
    delay(delaymS);
  }
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);          //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  pinMode(LED1, OUTPUT);                     //for PCB LED
  pinMode(13, OUTPUT);                       //for Pro Mini LED, Pin13
 }
