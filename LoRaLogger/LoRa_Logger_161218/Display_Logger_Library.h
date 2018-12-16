//https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/  - download LiquidCrystal_V1.2.1.zip//Display_Logger_Library.h

/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 12/12/2018

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.
*****************************************************************************************************************************
*/


#ifdef UseDisplay

#ifdef UseI2CLCD
#include <LiquidCrystal_I2C.h>              //https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/  - download LiquidCrystal_V1.2.1.zip                                      
LiquidCrystal_I2C disp(LCDI2C_Address, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //Set the LCD I2C address and pins used
#endif


#ifdef UseSSD1306
#include "SSD1306Ascii.h"                   //https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire disp;
#endif


void writescreen_RSSI(int8_t tempRSSI)
{
  disp.setCursor(0, 2);
  disp.print(F("RSSI "));
  disp.print(tempRSSI );                   //print the SNR
  disp.print(F("dB  "));
}


void writescreen_SNR(int8_t tempSNR)
{
  disp.setCursor(0, 1);
  disp.print(F("SNR "));
  disp.print(tempSNR);                     //print the SNR
  disp.print(F("dB  "));
}

#endif
