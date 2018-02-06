//SD1306_Display_Test.ino
#define programname "SD1306 Display Test"
#define programversion "V1.1"
#define dateproduced "21/12/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 21/12/17

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

  This test program has written to test a 128x64 SD1306 OLED display at small and large text sizes
  monitor.

  This test program uses the ASCII only SD1306 driver library; //https://github.com/greiman/SSD1306Ascii

********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Connections

  The program uses the standard I2C connections on Arduino.
********************************************************************************************************************************
*/


#include <Wire.h>
#include "SSD1306Ascii.h"                            //https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiAvrI2c.h"
SSD1306AsciiAvrI2c disp;

#define I2C_ADDRESS 0x3C                             //address will be 0x3C or 0x3D
#define Serial_Monitor_Baud 38400                    //this is baud rate used for the Arduino IDE Serial Monitor


void loop(void)
{
  Serial.println();
  Serial.println(F("Running SD1306 Display Test"));

  disp.clear();
  screen1();
  delay(2000);

  disp.clear();
  screen2();
  delay(5000);

  disp.clear();
  screen3();
  delay(5000);
}


void screen1()
{
  Serial.println(F("Screen 1 - Small Text"));
  disp.set1X();
  Display_SetCurPos(0, 0);
  disp.print("SD1306 Display Test");
  Display_SetCurPos(0, 1);
  disp.print("www.LoRaTracker.uk");
  Display_SetCurPos(0, 3);
  disp.print("Small Text");
}


void screen2()
{
  Serial.println(F("Screen 2 - Small Text"));
  disp.set1X();
  disp.setCursor(0, 0);
  disp.print("LAT 51.48230  48M");
  disp.setCursor(0, 1);
  disp.print("LON -3.18136");
  disp.setCursor(0, 2);
  disp.print("9.2K 6D");
  disp.setCursor(0, 3);
  disp.print("SNR 8dB     1    SEG");
}


void screen3()
{
  Serial.println(F("Screen 3 - Large Text"));
  disp.set2X();
  disp.setCursor(0, 0);
  disp.print("51.48230");
  disp.setCursor(0, 2);
  disp.print("-3.18136");
  disp.setCursor(0, 4);
  disp.print("48M");
  disp.setCursor(0, 6);
  disp.print("9.2K 6D");
}


void Display_SetCurPos(byte lcol, byte lrow)
{
  lcol = lcol * 6;
  disp.setCursor(lcol, lrow);
}


void setup(void)
{
  Serial.begin(Serial_Monitor_Baud);
  Serial.println();
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  Wire.begin();
  disp.begin(&Adafruit128x64, I2C_ADDRESS);
  disp.setFont(System5x7);
}




