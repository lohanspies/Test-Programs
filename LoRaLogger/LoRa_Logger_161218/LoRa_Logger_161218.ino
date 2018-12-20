#define programname "LoRa_Logger_161218"
#define programversion "V1.1"
#define authorname "Stuart Robinson - www.LoRaTracker.uk"
#include <Arduino.h>

/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 16/12/2018

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit
  permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.
*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Program operation

  This program will receive a raw LoRa packet based on the LoRa settings in the 'Settings.h' file.

  The SNR, RSSI and packet length will be displayed on the serial monitor, together with the ASCII of HEX contents of the
  packet. Any ASCII control characters less then 0x20 are replaced with HEX values in brackets. The packet data can also
  be logged to an attached SD card as text or hexadecimal, the format is the same as the serial monitor output.

  Depending on the time gaps between packets you may need a faster processor than an 8Mhz Arduino Pro Mini to ensure that
  a packet is printed and logged before the next one arrives.

  You need to specify the pins that connect to the LoRa device and SD card select in the 'Settings.h' file

  Do not forget to fit an antenna to the LoRa device, you can destroy it if you use it without and antenna
********************************************************************************************************************************

  To do:
  Done - Detect and report phantom packets
  Done - Print log number on display

  Add function to close log on switch press interrupt
  Done Check while (!lora_readRXreadyDIO0()); is working


  Changes:
  11/12/18 Working with no RX buffer, FIFO read and display
  11/12/18 Serial output matches SD log output, display working, 21004 bytes (68%), 1410 bytes (68%)


*************************************************************************************************************************
*/

//Program Variables

boolean SD_Found = false;                   //set if SD card found at program startup
uint8_t lognumber;
unsigned long total_errors = 0;

#include "Pin_Definitions.h"

#include "Settings.h"

#include <SPI.h>
#include <Wire.h>

#include "LoRa5.h"

#ifdef USESD
#include <SdFat.h>                           //https://github.com/greiman/SdFat
SdFat SD;
File logFile;
char filename[] = "Log000.txt";
#include "SD_Logger_Library.h"
#endif

#ifdef UseDisplay
#include "Display_Logger_Library.h"
#endif



void loop()
{
  lora_RXONLoRa();                             //turn on receiver ready for next packet and clear flags

#ifdef USESD
  logFile.println();
  logFile.println(F("Listening"));
  logFile.flush();
  logFile.close();
  delay(50);
#endif
  Serial.println();
  Serial.println(F("Listening "));
  Serial.flush();

  while (!lora_readRXreadyDIO0())
  {
    check_Switch();                              //check for switch press, meaning close log
  }

  lora_ReadPacketDetails();                    //read packet details

  PacketOK = lora_IsPacketValid();             //get packet errors, update counts

  digitalWrite(LED1, HIGH);
  digitalWrite(BUZZ, HIGH);

  Serial.println();
  print_uptime();
  Serial.println();

  Serial.print(F("PacketRX "));

#ifdef USESD
  if (SD_Found)
  {
    delay(100);

    if (SD.exists(filename))
    {
      logFile = SD.open(filename, FILE_WRITE);
    }
    else
    {
      Serial.println();
      Serial.print(filename);
      Serial.println(F(" - not found !"));
      SD.begin(SD_CS);
    }

    logFile.println();
    SD_printUptime();
    logFile.println();
    logFile.print(F("PacketRX "));
  }

#endif

  lora_PrintReceptionInfo();
  Serial.println();

#ifdef UseDisplay
  writescreen_SNR(lora_PacketSNR);
  writescreen_RSSI(lora_PacketRSSI);
#endif

#ifdef USESD
  SD_PrintReceptionInfo();
  logFile.println();
#endif

  check_Packet();                                 //a packet has arrived, so check it

  digitalWrite(BUZZ, LOW);
  digitalWrite(LED1, LOW);
}


void check_Switch()
{
#ifdef USESD
  if (!(digitalRead(Switch1)))              //if switch is pressed, close log and halt
  {
    Serial.print(F("Switch Pressed - Closing log "));
    Serial.println(filename);


    logFile = SD.open(filename, FILE_WRITE);
    logFile.print(F("Switch Pressed - Closing log "));
    logFile.close();

#ifdef UseDisplay
    disp.clear();
    disp.setCursor(0, 0);
    disp.print("Closed Log");
    disp.setCursor(0, 1);
    disp.print(filename);
#endif

    while (1);                  //loop forever
  }
#endif  
}

  void process_Packet()
  {
    //process the received packet

#ifdef SERIALASCII
    lora_RXFIFOPrint(0);
    Serial.println();
#endif

#ifdef SERIALHEX
    lora_RXFIFOPrint(1);
    Serial.println();
#endif

#ifdef USESD

#ifdef LOGASCII
    SD_RXFIFOPrint(0);                  //print RX FIFO to SD as ASCII
    logFile.println();
#endif

#ifdef LOGHEX
    SD_RXFIFOPrint(1);                   //print RX FIFO to SD as HEX
    logFile.println();
#endif

    SD_PrintTotals();
    logFile.println();

#endif

    PrintTotals();
  }


  void check_Packet()
  {
    //checks the packet details

    byte regtemp;

    regtemp = lora_Read(lora_RegIrqFlags);
    Serial.print(F("ReqIrqFlags "));
    Serial.println(regtemp, HEX);

#ifdef USESD
    logFile.print(F("ReqIrqFlags "));
    logFile.println(regtemp, HEX);
#endif

    if (HeaderOK)
    {
      Serial.print(F("HeaderOK,"));
    }
    else
    {
      Serial.print(F("HeaderError,"));
    }

    if (PayloadhasCRC)
    {
      Serial.print(F("PayloadhasCRC,"));
    }
    else
    {
      Serial.print(F("NoPayloadCRC,"));
    }

    if (CRCError && PayloadhasCRC)
    {
      Serial.print(F("CRCError,"));
    }

    if (HeaderOK && !CRCError && PayloadhasCRC)
    {
      Serial.print(F("CRCOK,"));
    }

    if (PacketOK)
    {
      Serial.print(F("PacketValid"));
    }

    if ((HeaderOK) && (!PayloadhasCRC) && (!CRCError))             //if Payload has CRC bit clear CRC error flag is low, packet is phantom
    {
      Serial.print(F("Possible Packet Phantom!"));
    }

    Serial.println();


#ifdef USESD

    if (HeaderOK)
    {
      logFile.print(F("HeaderOK,"));
    }
    else
    {
      logFile.print(F("HeaderError,"));
    }

    if (PayloadhasCRC)
    {
      logFile.print(F("PayloadhasCRC,"));
    }
    else
    {
      logFile.print(F("NoPayloadCRC,"));
    }

    if (CRCError && PayloadhasCRC)
    {
      logFile.print(F("CRCError,"));
    }

    if (HeaderOK && !CRCError && PayloadhasCRC)
    {
      logFile.print(F("CRCOK,"));
    }

    if (PacketOK)
    {
      logFile.print(F("PacketValid"));
    }

    if ((HeaderOK) && (!PayloadhasCRC) && (!CRCError))             //if Payload has CRC bit clear CRC error flag is low, packet is phantom
    {
      logFile.println(F("Possible Packet Phantom"));
    }
    logFile.println();
#endif

    process_Packet();

#ifdef UseDisplay
    disp.setCursor(0, 0);
    if (PacketOK)
    {
      disp.print(F("Packet OK    "));
    }
    else
    {
      disp.print(F("Packet Error "));
    }
    disp.setCursor(16, 0);
    disp.print(lognumber);
#endif
  }


  void PrintTotals()
  {
    Serial.print(F("OK Packets "));
    Serial.print(lora_PacketOKCount);
    Serial.print(F("  CRC Errors "));
    Serial.print(lora_RXCRCerrorCount);
    Serial.print(F("  Header Errors "));
    Serial.print(lora_RXHeadererrorCount);
    Serial.print(F("  Phantom Packets "));
    Serial.println(lora_PhantomPacketCount);

#ifdef UseDisplay
    total_errors = lora_RXCRCerrorCount + lora_RXHeadererrorCount + lora_PhantomPacketCount;
    disp.setCursor(0, 3);
    disp.print(F("OK "));
    disp.print(lora_PacketOKCount);
    disp.print(F(" Errors "));
    disp.print(total_errors);
#endif
  }


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


  void Setup_LoRaMode()
  {
    lora_SetFreqInt(Frequency, CalibrationOffset);                      //Setup the LoRa frequency
    lora_SetModem2(Bandwidth, SpreadingFactor, CodeRate, Explicit);  //Setup the LoRa modem parameters
  }


  void print_uptime()
  {
    uint32_t uptime;
    uptime = millis() / 1000;
    Serial.print(uptime);
    Serial.print(F(" Seconds"));
  }


  void setup()
  {

    Serial.begin(115200);                      //setup Serial console ouput

    Serial.println(__FILE__);
    Serial.print(F("Compiled "));
    Serial.print(__TIME__);
    Serial.print(F(" "));
    Serial.println(__DATE__);
    Serial.println(F(authorname));
    Serial.println();

    pinMode(LED1, OUTPUT);                     //setup pin for PCB LED
    led_Flash(2, 250);

    SPI.begin();                               //initialize SPI:
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

    //pinMode (lora_NSS, OUTPUT);                //LoRa Device select line
    //pinMode(lora_NReset, INPUT);              //LoRa Device reset line
    //pinMode (lora_DIO0, INPUT);                //LoRa DIO0 pin, used for RX ready
    //pinMode (BUZZ, OUTPUT);                    //setup Buzzer pin
    pinMode (Switch1, INPUT_PULLUP);
    digitalWrite(lora_NSS, HIGH);
    

#ifdef UseDisplay

#ifdef UseSSD1306
    disp.begin(&Adafruit128x64, SSD1306_Address);
    disp.setFont(Adafruit5x7);
    disp.set1X();
#endif

#ifdef UseI2CLCD
    disp.begin(20, 4);                              //initialize the lcd for 20 chars 4 lines
    disp.backlight();                               //turn on backlight, if option available
#endif

    Serial.println(F("Display Option Selected"));
    disp.clear();
    disp.setCursor(0, 0);
    disp.print(F("Starting"));
#endif


#ifdef UseDisplay
    Serial.println(F("Display Started"));
    disp.clear();
    disp.setCursor(0, 0);
    disp.print(F("Display Started"));
#endif

    if (!lora_CheckDevice())
    {
      Serial.println(F("LoRa Error"));
      led_Flash(40, 50);
    }

    lora_Setup(lora_NSS, lora_NReset, lora_DIO0);
    Setup_LoRaMode();

#ifdef EnableTone
    digitalWrite(LED1, HIGH);                   //turn on LED
    digitalWrite(BUZZ, HIGH);
    lora_Tone(1000, 1000, 10, lora_DIO2);                  //Transmit an FM tone, 1000hz, 1000ms, 10dBm
    digitalWrite(LED1, LOW);                    //LED is off
    digitalWrite(BUZZ, LOW);
    delay(1000);
    Setup_LoRaMode();
#endif

    Serial.println();

#ifdef USESD
    lognumber = setup_SDLOG();                   //setup SD card
    Serial.print(F("Lognumber "));
    Serial.println(lognumber);
#endif

    lora_Setup(lora_NSS, lora_NReset, lora_DIO0);
    Setup_LoRaMode();
    lora_RXONLoRa();                             //LoRa device into receive mode
    Serial.println();
    lora_Print();                                //so we can see full set of receive registers
    Serial.println();

#ifdef UseDisplay
    disp.clear();
    disp.setCursor(0, 0);
#ifdef USESD
    if (!SD_Found)
    {
      disp.print(F("SD Card Fail"));
      delay(2000);
    }

#endif

    disp.clear();
    disp.setCursor(0, 0);
    disp.print(F("Logger Ready"));
    disp.setCursor(16, 0);
    disp.print(lognumber);
#endif

    Serial.flush();

    lora_PrintLoRaSettings();

#ifdef UseSD
    SD_PrintLoRaSettings();
#endif

  }




