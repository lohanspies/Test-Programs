#define programname "ILI9341_Display_Test"
#define programversion "V1.0"
#define dateproduced "12/12/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

/*
*****************************************************************************************************************************

LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 12/12/2017

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

This test program has been written to check that an ILI9341 is displaying text of different sizes correctly and with proper
spacing. It displays sample text in text scales from 1 to 4. The starting font is the SystemFont5x7. The rows and columns you
display in each text scale are;

1 - 24 Rows, 54 Columns
2 - 12 Rows, 26 Columns
3 - 8 Rows, 18 Columns
5 - 5 Rows, 13 Columns

The program was written primarily for testing the ILI9341 for the LoRaTracker LCD display backpack which is ATMega328 based,
you will need to download and install the following library;

https://github.com/marekburiak/ILI9341_due


********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses 3 user defined pins on the Arduino, DISP_CS, DISP_DC, DISP_RESET you can explicitly define these pins by 
removing the two // characters in front of the 3 define lines below.

In addition the following ILI9341 pins should be connected 

ILI9341PIN    Arduino ATMEGA328
SCK           13
MISO          12
MOSI          11
LED           VCC 3.3V   

********************************************************************************************************************************
*/

//#define DISP_CS 10
//#define DISP_RESET 9
//#define DISP_DC 8



/*
***********************************************************************************************************************************************
Board Definitions

As an alternative to explicitly defining the Arduino pins required, there are pre-defined board definition files for the LoRaTracker boards
included in the LoRaTracker Library;

https://github.com/LoRaTracker/LoRaTracker-Library

Select (include) the board definition file you require by removing the // characters before the appropriate include line in the list below
***********************************************************************************************************************************************
*/

#include "BackpackV4 _Board_Definitions.h"
//#include "BackpackV3 _Board_Definitions.h"


#include "SPI.h"

#include <ILI9341_due.h>                 //https://github.com/marekburiak/ILI9341_due
#include <ILI9341_due_config.h>

ILI9341_due disp = ILI9341_due(DISP_CS, DISP_DC, DISP_RESET);
#include "fonts\SystemFont5x7.h"


#define Serial_Monitor_Baud 38400        //this is baud rate used for the Arduino IDE Serial Monitor

unsigned long start_timems;
unsigned long end_timems;
byte textscale=1;


void cursorToXY(byte lcol, byte lrow)
{
unsigned int col, row, linespacing;
disp.cursorToXY((lcol*6*textscale),(lrow*10*textscale));
}


void setTextScale(byte lscale)
{
disp.setTextScale(lscale);
textscale = lscale;
disp.setTextLineSpacing(lscale);
disp.setTextLetterSpacing(1);
}


void loop()
{
  
  disp.fillScreen(ILI9341_BLACK);
  ILI9341_Size1();
  delay(2000);
  disp.fillScreen(ILI9341_BLACK);
  ILI9341_Size2();
  delay(2000);
  disp.fillScreen(ILI9341_BLACK);
  ILI9341_Size3();
  delay(2000);
  disp.fillScreen(ILI9341_BLACK);
  ILI9341_Size4();
  delay(2000);
  disp.fillScreen(ILI9341_BLACK);
  delay(1000);
}


void ILI9341_Size1()
{
  setTextScale(1);
  disp.setTextColor(ILI9341_WHITE);
  cursorToXY(0, 0);
  disp.print("Size 1 7890124567890123456789012345678901234567890123");
  disp.print("");
  cursorToXY(7, 1);
  disp.print("7890124567890123456789");
  disp.print("012345678901234567890123");
  cursorToXY(0, 4);
  disp.print("24 Rows   54 Columns");
  cursorToXY(0, 23);
  disp.print("Row 23");
}


void ILI9341_Size2()
{
  setTextScale(2);
  cursorToXY(0, 0);
  disp.setTextColor(ILI9341_YELLOW);
  disp.print("Size 2 7890124567890123456");
  cursorToXY(7, 1);
  disp.print("7890124567890123456");
  cursorToXY(0, 4);
  disp.print("12 Rows   26 Columns");
  cursorToXY(0, 11);
  disp.print("Row 11");
}


void ILI9341_Size3()
{
  setTextScale(3);
  cursorToXY(0, 0);
  disp.setTextColor(ILI9341_CYAN);
  disp.print("Size 3 7890124567");
  cursorToXY(7, 1);
  disp.print("7890124567");
  cursorToXY(0, 4);
  disp.print("8 Rows");
  cursorToXY(0, 5);
  disp.print("18 Columns");
  cursorToXY(0, 7);
  disp.print("Row 7");
}


void ILI9341_Size4()
{
  setTextScale(4);
  cursorToXY(0, 0);
  disp.setTextColor(ILI9341_GREEN);
  disp.print("Size 4 789012");
  cursorToXY(7, 1);
  disp.print("789012");
  cursorToXY(0, 2);
  disp.print("0123456789012");
  cursorToXY(0, 3);
  disp.print("5 Rows");
  cursorToXY(0, 4);
  disp.print("13 Columns");
  cursorToXY(0, 5);
  disp.print("Row 5");
}


void display_Test()
{
  //a standard display routine test. 
  unsigned long startuS = 0, enduS =0, texttimeuS = 0, cleartimeuS = 0;
  Serial.println(F("display_Test()"));

  startuS = micros();
  disp.fillScreen(ILI9341_BLACK);
  cleartimeuS = micros() - startuS;
  
  startuS = micros();
  setTextScale(3);
  cursorToXY(0, 0);

  disp.print(F("ILI9341     "));    //padded to 12 characters
  cursorToXY(0, 1);
  disp.print("Ready");
  texttimeuS = micros() - startuS;
  cursorToXY(0, 2);
  
  disp.print(F("Text  "));
  disp.print(texttimeuS);
  disp.print(F("uS"));
 
  Serial.print(F("Text "));
  Serial.print(texttimeuS);
  Serial.println(F("uS"));
  
  cursorToXY(0, 3);
  
  disp.print(F("Clear "));
  disp.print(cleartimeuS);
  disp.print(F("uS"));
    
  Serial.print(F("Clear "));
  Serial.print(cleartimeuS);
  Serial.println(F("uS"));
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);                                   //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
  
  pinMode(DISP_RESET, OUTPUT);             //setup pin for display reset
  digitalWrite(DISP_RESET, HIGH); 
  disp.begin();
  disp.setTextLetterSpacing(1);
  disp.setRotation(1);
  disp.setFont(SystemFont5x7);
  disp.fillScreen(ILI9341_BLACK);
  setTextScale(3);
  disp.println("Display Backpack");
  disp.println("www.LoraTracker.uk");
  disp.println();
  delay(2000);
  display_Test();
  delay(2000);
 
}

