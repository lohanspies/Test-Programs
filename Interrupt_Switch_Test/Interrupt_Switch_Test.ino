#define programname "Interrupt_Switch_Test"
#define programversion "V1.1"
#define dateproduced "29/11/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#include "Program_Definitions.h"                     //part of LoRaTracker library
#define LoRa_Device_in_MB1                           //required if board is using Mikrobus sockets

/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 29/11/2017

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

The program sets an interrupt to activate when a switch is pressed. When the switch is held down the LED stays on. Program 
written for ATmega328 (UNO & Pro Mini) so may need changes for other processors. 
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

You can explicitly define the required pin below by removing the two // characters in front of the #defines 
********************************************************************************************************************************
*/

#define SWITCH1 3                    //Arduino pin number for Switch, should be pulled low with switch pressed. 
#define LED1 10                      //Arduino pin number for LED, when high LED should be on.


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


#include "PinChangeInterrupt.h"                    //https://github.com/NicoHood/PinChangeInterrupt

#define Serial_Monitor_Baud 38400          //this is baud rate used for the Arduino IDE Serial Monitor

unsigned long SWITCH1_Interrupt_Count = 0;


void loop()
{

}


void interrupt_detected_SWITCH1()
{
 SWITCH1_Interrupt_Count++; 
 digitalWrite(LED1, !digitalRead(LED1));             //toggle the LED
 Serial.print("SWITCH1_Interrupt_Count ");                          
 Serial.println(SWITCH1_Interrupt_Count);
 delay(500);
}


void setup_interrupts()
{
  Serial.println("setup_interrupts()");
  attachPCINT(digitalPinToPCINT(SWITCH1), interrupt_detected_SWITCH1, CHANGE);
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
  Serial.begin(Serial_Monitor_Baud);                       //setup Serial console ouput
  Serial.println();
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));

  pinMode(LED1, OUTPUT);                     //for PCB LED
  pinMode(13, OUTPUT);                       //for Pro Mini LED, Pin13
  led_Flash(5,100);
  
  pinMode(SWITCH1, INPUT_PULLUP);
  
  setup_interrupts();
  delay(500);
}
