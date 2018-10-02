#define programname "I2C_FRAM_Memory_Test"
#define programversion "V1.1"
#define dateproduced "29/11/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#include "Program_Definitions.h"                     //part of LoRaTracker library

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

This program tests that an MB85RC16PNF FRAM is working and retaining memory contents when power is removed. This FRAM is 
non-voltatile storage with a write endurance of 10,000,000,000 write cycles.

The serial monitor will first display the results of an I2C Device scan which should list 8 devices found from 0x50 to
0x57, these are each of the eight 256 byte pages of the FRAM.

The program will read and print the last saved float value (see details below) such as;

Memory at Reset 1.247135

Next the program will clear the entire memory to zeros, then write 'Start' and 'End' at the beginning and end of memory
repectivly then print the entire memory contents to the serila monitor. This can be used as a check that the memory is
indeed a 16Kbit (2Kbyte) device.  

Next the program start writing a float value to the FRAM and reading it back and display it like this;

Writing Memory as 1.247135   Reading Memory as 1.247135 - Memory Errors 0

If you press reset on the Pro Mini or remove the power just after a Writing Memory line is printed, the next time you
restart the program, apply the power etc, 

This value should match the value noted at reset or power off. 
 
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses the standard I2C connections on Arduino.
********************************************************************************************************************************
*/

#include "I2CFRAM_MB85RC16PNF.h"                     //LoRaTracker library file for MB85RC16PNF
#include <Wire.h>
#include "I2C_Scanner.h"

#define Serial_Monitor_Baud 38400          //this is baud rate used for the Arduino IDE Serial Monitor

void loop()
{
  float data, data1;
  unsigned long cleartimemS, errors=0;

  Serial.println(F("Testing Memory"));
  Serial.println();
  Memory_WriteByte(0, 'S');
  Memory_WriteByte(1, 't');
  Memory_WriteByte(2, 'a');
  Memory_WriteByte(3, 'r');
  Memory_WriteByte(4, 't');
  Memory_WriteByte(2045, 'E');
  Memory_WriteByte(2046, 'n');
  Memory_WriteByte(2047, 'd');  
  Print_Memory(0, 15);
  delay(2000);
  Print_Memory(16, 2047);
  delay(2000);  

  data = Memory_ReadFloat(16);
  
  Serial.print(F("Memory at Reset "));
  Serial.println(data,6);
  
  Serial.print(F("Clearing first 256 bytes of Memory "));
  cleartimemS = millis();
  Memory_Set(0, 0xff, 0);
  Serial.print(F(" - Done - "));
  cleartimemS = millis() - cleartimemS;
  Serial.print(cleartimemS);
  Serial.println(F("mS"));
  Print_Memory(0, 255);

  data = 1;

  while (1)
  {
    data = data * 1.012345;
    if (data > 1000000)
    {
    data = 1;   
    }
    Serial.print(F("Writing Memory as "));
    Serial.print(data,6);
    Memory_WriteFloat(0, data);
    Serial.print(F("   Reading Memory as "));
    data1 = Memory_ReadFloat(0);
    Serial.print(data1,6);
    if (data != data1)
    {
    errors++;
    }
    Serial.print(F(" - Memory Errors ")); 
    Serial.print(errors);
    Serial.println();
    delay(1000);
  }

}



void setup()
{
  Serial.begin(Serial_Monitor_Baud);                                   //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  setup_I2CScan();
  run_I2CScan();

  Memory_Start();
}


