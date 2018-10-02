#define programname "LoRa_Module_Test"
#define programversion "V2.0"
#define dateproduced "02/10/2018"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>


/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 02/10/2018

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit
  permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose
  and free from errors.
*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Program operation

  This test program has been written to check that a connected LoRa module such as Dorji DRF127x or Hope RFM9x is accessible
  over the SPI interface. It prints the contents of the LoRa registers and the LoRa version whic is 0x12 for most devices.

  The program then prints the default frequency and attempts to change the frequency to a new value. The program then reads back the
  changed frequency back and prints what its been changed to so you can confirm the LoRa device is responding and can be
  written to and read from correctly.

  If you have an LED connect it to the pin defined by LED1. Connect the LED so that when the pin is high the LED is on.
  When the program starts there will be a couple of brief LED flashes. If the LoRa device can be accessed the LED will be
  on solid for a couple of seconds. If the LoRa device cannot be accessed the LED will flash rapidly for 2 seconds.

  The program then repeats the above sequence.

  Do not forget to fit an antenna to the LoRa device, you can destroy it if you turn on transmit without an antenna, this
  program however does not put the LoRa device into transmit or receive mode, its just a register read and write test.

  The LoRa registers after reset should look like this;

  LoRa Registers after reset
  Reg    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
  0x00  00 09 1A 0B 00 52 6C 80 00 4F 09 2B 20 08 02 0A
  0x10  FF 70 15 0B 28 0C 12 47 32 3E 00 00 00 00 00 40
  0x20  00 00 00 00 05 00 03 93 55 55 55 55 55 55 55 55
  0x30  90 40 40 00 00 0F 00 00 00 F5 20 82 FF 02 80 40
  0x40  00 00 12 24 2D 00 03 00 04 23 00 09 05 84 32 2B
  0x50  14 00 00 12 00 00 00 0F E0 00 0C FE 07 00 5C 78
  0x60  00 19 0C 4B CC 0E BF 20 04 47 AF 3F BF 00 87 0B
  0x70  D0 01 0F 00 00 00 00 00 00 00 00 00 00 00 00 00

  Registers, 06, 07 and 08 are the frequency, 42 is the version ID.

********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Connections

  The program uses the hardware SPI interface on an Arduino to connect to the LoRa device, so the SPI SCK, MOSI and MISO pins need
  to be connected. The location of these pins varies depending on the Arduino, for a UNO or Pro Mini they are;

  SCK 13
  MISO 12
  MOSI 11

  The test program needs a minimum of two extra pins connected to act as lora device chip select (lora_NSS). The other pin
  that should be connected is the RESET pin on the LoRa device.

  Thus to make the program work the only pins you need to connect on the LoRa device are;

  SCK
  MISO
  MOSI
  NSS (lora_NSS)
  RESET (lora_RESET)
  3.3V
  GND

  Leave all other pins not connected. The program does not turn on transmit at any time, but in case you forget when testing
  further be sure to fit an antenna.
********************************************************************************************************************************
*/




#define lora_NSS 10                   //Arduino pin number for device select on LoRa device
#define lora_NReset 9                 //Arduino pin number for RESET pin on LoRa device
#define LED1 8                        //Arduino pin number for LED, when high LED should be on.  

const byte lora_RegFrMsb = 0x06;      //LoRa device register definition
const byte lora_RegFrMid = 0x07;      //LoRa device register definition
const byte lora_RegFrLsb = 0x08;      //LoRa device register definition
const byte lora_RegVersion = 0x42;    //LoRa device register definition
const float Frequency = 434.400;      //frequency to set in Megahertz

#include <SPI.h>

#define Serial_Monitor_Baud 9600       //this is baud rate used for the Arduino IDE Serial Monitor



void loop()
{
  uint8_t reg_data;
  float tempfrequency;
  Serial.println(F("LED Flash"));
  Serial.println();
  led_Flash(2, 100);

  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  Serial.println(F("Checking LoRa Device"));
  Serial.println();

  if (lora_CheckDevice() == true)
  {
    digitalWrite(LED1, HIGH);
    Serial.println(F("LoRa Device Present"));
    lora_printVersion();
    Serial.println();
    lora_ResetDev();                               //ensure registers are in initial state
    Serial.println(F("LoRa Registers after reset"));
    lora_Print();                                  //initial print of registers
    Serial.println();
    Serial.println();
    tempfrequency = lora_GetFreq();
    Serial.print(F("Default frequency "));
    Serial.print(tempfrequency, 3);
    Serial.println(F("MHz"));
    Serial.print(F("Write new frequency "));
    Serial.print(Frequency, 3);
    Serial.println(F("MHz"));
    lora_SetFreq(Frequency, 0);
    tempfrequency = lora_GetFreq();
    Serial.print(F("Changed frequency "));
    Serial.print(tempfrequency, 3);
    Serial.println(F("MHz"));
    Serial.println();

  }
  else
  {
    Serial.println(F("LoRa Device Not Found"));
    Serial.println();
    lora_Print();
    Serial.println();
    led_Flash(40, 25);
  }

  SPI.end();
  Serial.println();
  Serial.println();
  delay(1500);
  digitalWrite(LED1, LOW);
}


void lora_printVersion()
{
  byte reg_data;
  reg_data = lora_Read(lora_RegVersion);
  Serial.print(F("LoRa device version 0x"));

  if (reg_data < 0x10)
  {
    Serial.print(F("0"));
  }

  Serial.print(reg_data, HEX);
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


void lora_Print()
{
  //prints the contents of LoRa registers to serial monitor
  byte Loopv1, Loopv2, Reg, RegData;

  Serial.print(F("Reg    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F"));
  Serial.println();
  Reg = 0;
  for (Loopv1 = 0; Loopv1 <= 7; Loopv1++)
  {
    Serial.print(F("0x"));
    Serial.print(Loopv1, HEX);              //print the register number
    Serial.print(F("0  "));
    for (Loopv2 = 0; Loopv2 <= 15; Loopv2++)
    {
      RegData = lora_Read(Reg);
      if (RegData < 0x10) {
        Serial.print(F("0"));
      }
      Serial.print(RegData, HEX);           //print the register number
      Serial.print(F(" "));
      Reg++;
    }
    Serial.println();
  }
}


void lora_SetFreq(float lora_LFreq, float lora_LOffset)
{
  //set the LoRa frequency
  lora_LFreq =  lora_LFreq + lora_LOffset;
  byte lora_LFMsb, lora_LFMid, lora_LFLsb;
  long lora_LLongFreq;
  lora_LLongFreq = ((lora_LFreq * 1000000) / 61.03515625);
  lora_LFMsb =  lora_LLongFreq >> 16;
  lora_LFMid = (lora_LLongFreq & 0x00FF00) >> 8;
  lora_LFLsb =  (lora_LLongFreq & 0x0000FF);
  lora_Write(lora_RegFrMsb, lora_LFMsb);
  lora_Write(lora_RegFrMid, lora_LFMid);
  lora_Write(lora_RegFrLsb, lora_LFLsb);
}


void lora_Write(byte Reg, byte RegData)
{
  //write a byte to a LoRa register
  digitalWrite(lora_NSS, LOW);       //set NSS low
  SPI.transfer(Reg | 0x80);          //mask address for write
  SPI.transfer(RegData);             //write the byte
  digitalWrite(lora_NSS, HIGH);      //set NSS high
}


byte lora_Read(byte Reg)
{
  //read a byte from a LoRa register
  byte RegData;
  digitalWrite(lora_NSS, LOW);       //set NSS low
  SPI.transfer(Reg & 0x7F);          //mask address for read
  RegData = SPI.transfer(0);         //read the byte
  digitalWrite(lora_NSS, HIGH);      //set NSS high
  return RegData;
}


void lora_ResetDev()
{
  //resets the LoRa device
  digitalWrite(lora_NReset, LOW);    //take reset line low
  delay(5);
  digitalWrite(lora_NReset, HIGH);   //take it high
}

byte lora_CheckDevice()
{
  //check there is a device out there, writes a register and read back
  byte RegData;
  lora_Write(lora_RegFrMid, 0xAA);
  RegData = lora_Read(lora_RegFrMid);    //Read RegFrMid
  if (RegData == 0xAA )
  {
    return true;
  }
  else
  {
    return false;
  }
}

float lora_GetFreq()
{
  //get the current set LoRa frequency
  byte lora_LFMsb, lora_LFMid, lora_LFLsb;
  unsigned long lora_Ltemp;
  float lora_Ltemp1;
  lora_LFMsb = lora_Read(lora_RegFrMsb);
  lora_LFMid = lora_Read(lora_RegFrMid);
  lora_LFLsb = lora_Read(lora_RegFrLsb);
  lora_Ltemp = ((lora_LFMsb * 0x10000ul) + (lora_LFMid * 0x100ul) + lora_LFLsb);
  lora_Ltemp1 = ((lora_Ltemp * 61.03515625) / 1000000ul);
  return lora_Ltemp1;
}


void setup()
{
  pinMode(LED1, OUTPUT); 			               //for LED
  pinMode(lora_NReset, OUTPUT);			         //LoRa Device reset line
  pinMode (lora_NSS, OUTPUT);			           //LoRa Device select line
  digitalWrite(lora_NSS, HIGH);
  digitalWrite(lora_NReset, HIGH);

  Serial.begin(Serial_Monitor_Baud);         //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
}
