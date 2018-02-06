#define programname "LoRa_Receiver_040218"
#define programversion "V1.0"
#define authorname "Stuart Robinson - www.LoRaTracker.uk"

#include <Arduino.h>
//#include "Program_Definitions.h"
#include "LoRa_Receiver_Logger_Settings.h"

/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 04/02/2018

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

  This program will receive a raw LoRa packet based on the LoRa settings in the 'Basic_Receiver_Logger_Settings.h' file.

  The SNR, RSSI and packet length will be displayed on the serial monitor, together with the ASCII contents of the packet.
  Any ASCII control characters less then 0x20 are replaced with spaces. The packet data can also be logged to an attached
  SD card as text or hexadecimal.

  When a packet is received and it passes CRC check the 'process_Packet()' function is called where the printing and
  logging is carried out. The received packet is stored in a byte buffer lora_RXBUFF[] if you wish to do further analysis.
  Byte variables lora_RXStart and lora_RXEnd point to the start and end of the packet data in the buffer.

  Depending on the time gaps between packets you may need a faster processor than an 8Mhz Arduino Pro Mini to ensure that
  a packet is printed and logged before the next one arrives.

  You need to specify the pins that connect to the LoRa device and SD card select in the 'Basic_Receiver_Logger_Settings.h' file

  Do not forget to fit an antenna to the LoRa device, you can destroy it if you use it without and antenna
********************************************************************************************************************************

  To do:
  Check that print of packet contents to terminal does not send control characters

******************************************************************************************************
*/


//Program Variables

//unsigned long Packets;                      //running count of Mode packets received
boolean SD_Found = false;                     //set if SD card found at program startup
byte keypress;

#include <SPI.h>
#include "LoRaBasic.h"
#include <TimeLib.h>                         //https://github.com/PaulStoffregen/Time

#ifdef UseDisplay
#include <Wire.h>
#include "I2CDisplay.h"
I2CDisplay disp(Display_Address);            //create the display class
#endif

#ifdef UseSD
#include <SdFat.h>                           //https://github.com/greiman/SdFat
SdFat SD;
File logFile;
#endif


void loop()
{
  check_for_Packet();
}


byte check_for_Packet()
{
  //checks to see if a packet has arrived
  byte returnbyte;

#ifdef USEDIO_RXReady
  returnbyte = lora_readRXready2();
#else
  returnbyte = lora_readRXready();
#endif

  if (returnbyte == 64)
  {
    digitalWrite(LED1, HIGH);
    lora_ReadPacket();
    process_Packet();
    digitalWrite(LED1, LOW);
    lora_RXONLoRa();                                //ready for next and clear flags
    return 1;
  }

  if (returnbyte == 96)
  {
    Serial.println(F("CRC Error"));
#ifdef UseSD
    logFile.println(F("CRC Error"));
    logFile.flush();
#endif
    lora_RXONLoRa();                                 //ready for next
    lora_RXCRCerrorCount++;
  }
  return 0;                                          //no valid packet received
}



void Setup_LoRaMode()
{
  lora_SetFreq(Frequency, CalibrationOffset);                      //Setup the LoRa frequency
  lora_SetModem2(Bandwidth, SpreadingFactor, CodeRate, Explicit);  //Setup the LoRa modem parameters
  print_CurrentLoRaSettings();
}


void print_CurrentLoRaSettings()
{
  //prints the current LoRa settings, reads device registers
  float tempfloat;
  int tempint;
  byte regdata;
  unsigned long bw;

  tempfloat = lora_GetFreq();
  Serial.print(tempfloat, 5);
  Serial.print(F("MHz  ("));

  tempint = CalibrationOffset;
  tempfloat = (float)(tempint / 1000);
  Serial.print(tempfloat, 1);

  Serial.print(F("Khz)"));

  regdata = lora_Read(lora_RegModemConfig1);
  regdata = regdata & 0xF0;
  bw = lora_returnbandwidth(regdata);
  Serial.print(F("  BW"));
  Serial.print(bw);

  regdata = lora_Read(lora_RegModemConfig2);
  regdata = (regdata & 0xF0) / 16;
  Serial.print(F("  SF"));
  Serial.print(regdata);

  regdata = lora_Read(lora_RegModemConfig1);
  regdata = regdata & B00001110;
  regdata = regdata / 2; //move right one
  regdata = regdata + 4;

  Serial.print(F("  CR4/"));
  Serial.print(regdata);

  regdata = lora_Read(lora_RegModemConfig3);
  regdata = regdata & B00001000;
  Serial.print(F("  LDROPT_"));
  if (regdata == 8)
  {
    Serial.print(F("ON"));
  }
  else
  {
    Serial.print(F("OFF"));
  }

}



void process_Packet()
{
  //process received packet
  digitalWrite(LED1, HIGH);
  digitalWrite(BUZZ, HIGH);

#ifdef SERIALASCII
  Serial_WriteASCIIPacket();
#endif

#ifdef SERIALHEX
  Serial_WriteHEXPacket();
#endif

#ifdef LOGASCII
  SD_WriteASCIIPacket_Log();
#endif

#ifdef LOGHEX
  SD_WriteHEXPacket_Log();
#endif

#ifdef UseDisplay
  Display_WritePacket();
#endif

}


void Serial_WriteASCIIPacket()
{
#ifndef RAWONLY
  Serial_WriteTime();
  Serial_WritePacketInfo();
#endif
  lora_RXBuffPrint(PrintASCII);                      //print packet contents as ASCII to terminal
  Serial.println();
}


void Serial_WriteHEXPacket()
{
#ifndef RAWONLY
  Serial_WriteTime();
  Serial_WritePacketInfo();
#endif

  lora_RXBuffPrint(PrintHEX);                         //print packet contents as ASCII to terminal
  Serial.println();
}

void Serial_WritePacketInfo()
{
  Serial.print(F(","));
  lora_ReceptionInfo();
  Serial.print(F(",Len,"));
  Serial.print(lora_RXPacketL);
  Serial.print(F(","));
}

void Serial_WriteTime()
{
  if (hour() < 10)
  {
    Serial.write('0');
  }
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
}



void printDigits(int digits) {
  //utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(F(":"));
  if (digits < 10)
    Serial.print(F("0"));
  Serial.print(digits);
}


/*
*************************************************************************************
  Start SD card routines
*************************************************************************************
*/

#ifdef UseSD
void SD_WriteASCIIPacket_Log()
{
  byte index;

  if (SD_Found)
  {

#ifndef RAWONLY
    SD_WriteTime();
    SD_WritePacketInfo();
#endif

    for (index = lora_RXStart; index <= lora_RXEnd; index++)
    {
      logFile.write(lora_RXBUFF[index]);
    }
    logFile.write(13);
    logFile.write(10);
    logFile.flush();
  }
  else
  {
    Serial.println(F("No SD card !"));
  }
}


void SD_WriteHEXPacket_Log()
{
  byte index;
  byte bufferData;

  if (SD_Found)
  {

#ifndef RAWONLY
    SD_WriteTime();
    SD_WritePacketInfo();
#endif

    for (index = lora_RXStart; index <= lora_RXEnd; index++)
    {
      bufferData = lora_RXBUFF[index];
      if (bufferData < 0x10)
      {
        logFile.write('0');
      }

      logFile.print(bufferData, HEX);
      logFile.write(' ');
    }

    logFile.write(13);
    logFile.write(10);
    logFile.flush();
  }
  else
  {
    Serial.println(F("No SD card !"));
  }
}


void SD_WriteTime()
{
  if (hour() < 10)
  {
    logFile.write('0');
  }
  logFile.print(hour());
  logFile.write(':');
  if (minute() < 10)
  {
    logFile.write('0');
  }
  logFile.print(minute());
  logFile.write(':');
  if (second() < 10)
  {
    logFile.write('0');
  }
  logFile.print(second());
}


void SD_WritePacketInfo()
{
  logFile.print(F(",SNR,"));
  logFile.print(lora_PacketSNR);
  logFile.print(F("dB"));

  logFile.print(F(",RSSI,"));
  logFile.print(lora_PacketRSSI);
  logFile.print(F("dB"));

  logFile.print(F(",Len,"));
  logFile.print(lora_RXPacketL);
  logFile.print(F(","));
}

boolean setup_SDLOG()
{
  //checks if the SD card is present and can be initialised

  Serial.print(F("SD card..."));

  if (!SD.begin(SD_CS))
  {
    Serial.println(F("Failed, or not present"));
    SD_Found = false;
    return false;                                 //don't do anything more:
  }

  Serial.print(F("Initialized OK"));
  SD_Found = true;

  char filename[] = "Log000.txt";
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logFile = SD.open(filename, FILE_WRITE);
      break;                                      // leave the loop!
    }
  }

  Serial.print(F("...Writing to "));
  Serial.print(F("Log0"));
  Serial.write(filename[4]);
  Serial.write(filename[5]);
  Serial.println(F(".txt"));
  return true;
}


void SD_WriteCurrentLoRaSettings()
{
  //prints the current LoRa settings, reads device registers
  float tempfloat;
  int tempint;
  byte regdata;
  unsigned long bw;

  if (SD_Found)
  {
    tempfloat = lora_GetFreq();
    logFile.print(tempfloat, 5);
    logFile.print(F("MHz  ("));

    tempint = CalibrationOffset;
    tempfloat = (float)(tempint / 1000);
    logFile.print(tempfloat, 1);

    logFile.print(F("Khz)"));

    regdata = lora_Read(lora_RegModemConfig1);
    regdata = regdata & 0xF0;
    bw = lora_returnbandwidth(regdata);
    logFile.print(F("  BW"));
   logFile.print(bw);

    regdata = lora_Read(lora_RegModemConfig2);
    regdata = (regdata & 0xF0) / 16;
    logFile.print(F("  SF"));
    logFile.print(regdata);

    regdata = lora_Read(lora_RegModemConfig1);
    regdata = regdata & B00001110;
    regdata = regdata / 2; //move right one
    regdata = regdata + 4;

    logFile.print(F("  CR4/"));
    logFile.print(regdata);

    regdata = lora_Read(lora_RegModemConfig3);
    regdata = regdata & B00001000;
    logFile.print(F("  LDROPT_"));
    if (regdata == 8)
    {
      logFile.println(F("ON"));
    }
    else
    {
      logFile.println(F("OFF"));
    }
  }
  else
  {
    Serial.println(F("No SD card !"));
  }

}


void printlog_addleadingZero(byte temp)
{
  if (temp < 10)
  {
    logFile.print(F("0"));
  }
  logFile.print(temp);
}
#endif

/*
*************************************************************************************
  End SD card routines
*************************************************************************************
*/




/*
*************************************************************************************
  Start display routines - not yet implemented
*************************************************************************************
*/

#ifdef UseDisplay
void Display_WritePacket()
{
  byte index;
  disp.Clear();
  disp.SetCursorPosition(0, 0);

  for (index = lora_RXStart; index <= lora_RXEnd; index++)
  {
    disp.write(lora_RXBUFF[index]);
    if (index == 76)                                   //detect end of screen
    {
      break;
    }
  }
}

#endif
/*
*************************************************************************************
  End display routines
*************************************************************************************
*/


void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to show board is alive
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
  //needs no explanation I hope ...............

  pinMode(LED1, OUTPUT);                     //setup pin for PCB LED
  led_Flash(2, 250);

  Serial.begin(115200);                      //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(authorname));

  SPI.begin();                               //initialize SPI:
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  pinMode(lora_NReset, OUTPUT);              //LoRa Device reset line
  pinMode (lora_NSS, OUTPUT);                //LoRa Device select line
  pinMode (lora_DIO0, INPUT);                //LoRa DIO0 pin, used for RX ready
  pinMode (BUZZ, OUTPUT);                    //setup Buzzer pin
  digitalWrite(lora_NSS, HIGH);
  digitalWrite(lora_NReset, HIGH);

  if (!lora_CheckDevice())
  {
    Serial.println(F("LoRa Error"));
    led_Flash(40, 50);
  }

  lora_Setup();
  Setup_LoRaMode();

#ifdef CalibrateTone
  digitalWrite(LED1, HIGH);                   //turn on LED
  lora_Tone(1000, 3000, Mode_Power);          //Transmit an FM tone, 1000hz, 1000ms
  digitalWrite(LED1, LOW);                    //LED is off
  delay(1000);
  Setup_LoRaMode();
#endif

  Serial.println();

#ifdef UseSD
  setup_SDLOG();                               //setup SD and delay a bit to ensure any pending ints cleared
  SD_WriteCurrentLoRaSettings();
#endif

  lora_RXONLoRa();                             //LoRa device into receive mode

  Serial.println(F("Receiver ready"));

#ifdef UseDisplay
  disp.Clear();
  disp.print(F("Receiver ready"));
#endif

}




