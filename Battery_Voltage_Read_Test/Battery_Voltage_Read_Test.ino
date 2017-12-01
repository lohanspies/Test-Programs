#define programname "Battery_Voltage_Read_Test"
#define programversion "V1.1"
#define dateproduced "26/11/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

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

This test program has been written to check that hardware for reading the battery voltage has been assembled correctly such 
that it is funtional. 
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses 2 pins on the Arduino, SupplyAD and LED1. You can explicitly define them below by removing the two // characters
in front of the #define SupplyAD and #define LED1 lines.
********************************************************************************************************************************
*/


//#define SupplyAD A0                //Ressitor divider for battery connected here  
//#define LED1 13                    //pin for LED 
//#define ADMultiplier 10            //adjustment to convert AD value read into mV of battery voltage


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

#define Serial_Monitor_Baud 38400                    //this is baud rate used for the Arduino IDE Serial Monitor


void loop()
{
  Serial.println(F("LED Flash"));
  led_Flash(5, 100);
  print_SupplyVoltage();
  Serial.println(); 
  delay(1500);
}


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
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


unsigned int read_SupplyVoltage()
{
  //relies on 1V1 internal reference and 91K & 11K resistor divider
  //returns supply in mV @ 10mV per AD bit read
  unsigned int temp, SupplyVolts;
  byte index;

  analogReference(INTERNAL);
  for (index = 0; index <= 4; index++)                      //sample AD 3 times
  {
    temp = analogRead(SupplyAD);
    SupplyVolts = SupplyVolts + temp;
  }
  SupplyVolts = ((SupplyVolts / 5) * ADMultiplier);
  return SupplyVolts;
}


void print_SupplyVoltage()
{
  //get and display supply volts on terminal or monitor
  Serial.print(F("Supply Volts "));
  Serial.print(read_SupplyVoltage());
  Serial.println(F("mV"));
}


void setup()
{
 
  Serial.begin(Serial_Monitor_Baud);        //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  pinMode(LED1, OUTPUT);                   //for PCB LED
  pinMode(13, OUTPUT);                     //for Pro Mini LED, Pin13

  read_SupplyVoltage();                    //dummy read, first read can be inaccurate

}
