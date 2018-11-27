#define programname "LoRa_Signal_Generator"
#define programversion "V1.0"
#define dateproduced "30/12/2017"
#define aurthorname "Stuart Robinson"


/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 30/12/2017

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

This standalone test program (no libraries required) uses a LoRa device as an RF signal generator scanning between two frequencies
at a specified frequncy step. Its not very fast at scanning. Built to check a filters performance operation. Do not forget to fit
an antenna to the LoRa device you can destroy it if you use it without and antenna. Program generates a short FM tone at 
Start_Frequency when program starts.
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses the hardware SPI interface on the Arduino to connect to the LoRa device, so the SPI SCK, MOSI and MISO pins are
assumed to be connected. The test program needs a minimum of one extra pin connected to act as chip select. 
********************************************************************************************************************************
*/

#define lora_NSS 10                  //Arduino pin number for device select on LoRa device
#define lora_NReset 9                //Arduino pin number for reset pin on LoRa device, can be left not connected
#define lora_TonePin 7               //Arduino pin number connceted to DIO2 pin on LoRa device, used for FM audio tone generation 
#define lora_DIO0 2                  //Arduino pin number connceted to DIO0 pin on LoRa device, can be left not connected
#define LED1 8                       //Arduino pin number for LED, when high LED should be on.  


/***********************************************************************************************
// Test Settings - be careful when adjusting the frequency shift step size and delay, the output
// can break up if moved away from the given settings. 
***********************************************************************************************/

const unsigned long Start_Frequency = 818000000;        //start frequency of transmissions in hertz
const unsigned long End_Frequency =   918000000;        //start frequency of transmissions in hertz
const unsigned long Frequency_Shift = 5000;             //approx frequency shift in hertz per step
const byte TX_Power = 2;                                //transmit power in dBm, 2-17dBm supported 

const byte Deviation = 0x52;                            //typical deviation for audio tones, approx 5khz
const unsigned int Step_DelayuS = 5000;                 //delay in microseconds between frequency steps

unsigned long Frequency_Span;
unsigned long Frequency_Steps;
unsigned long Frequency_Output;
unsigned long StartmS;
unsigned long EndmS;
unsigned long Register_Shift;

#include <SPI.h>
#include "LoRaSG.h"                                     //LoRa routines
#define Serial_Monitor_Baud 115200                      //this is baud rate used for the Arduino IDE Serial Monitor

//#define DEBUG                                         //enable to see exdtra stuff in debug, slows scan down though   

void loop()
{
unsigned int i;
unsigned long temp;

lora_Write(lora_RegPllHop, 0xAD);                       //set fast hop mode, needed for fast changes of frequency
lora_TXONDirect(TX_Power);                              //TX on at spewcified TX power

Frequency_Span = End_Frequency - Start_Frequency;
Frequency_Steps = (Frequency_Span/Frequency_Shift);
Register_Shift = Frequency_Shift/61;


#ifdef DEBUG
Serial.print(F("Start_Frequency "));
Serial.println(Start_Frequency);

Serial.print(F("End_Frequency   "));
Serial.println(End_Frequency);

Serial.print(F("Frequency_Span  "));
Serial.println(Frequency_Span);

Serial.print(F("Frequency_Steps "));
Serial.println(Frequency_Steps); 

Serial.print(F("Register_Shift  "));
Serial.println(Register_Shift);
#endif 


Frequency_Output = Start_Frequency;

StartmS = millis();

for (i=0; i<=Frequency_Steps; i++)
{

Frequency_Output = Start_Frequency + (Frequency_Shift * i);

#ifdef DEBUG
//temp = Register_Shift * i;
//Serial.print(F("Index "));
//Serial.println(i);
//Serial.print(F("Register_Shift "));
//Serial.println(temp);

Serial.print(F("Frequency "));
Serial.println(Frequency_Output);
#endif

SetFreq();  
delayMicroseconds(Step_DelayuS);
}

EndmS = millis();
EndmS = EndmS - StartmS;
Serial.print(F("Scan Time "));
Serial.print(EndmS);
Serial.println(F("mS"));
Serial.println();
}


void SetFreq()
{
  Frequency_Output = ((uint64_t)Frequency_Output << 19) / 32000000;
  lora_Write(lora_RegFrMsb, (byte)(Frequency_Output >> 16));
  lora_Write(lora_RegFrMid, (byte)(Frequency_Output >> 8));
  lora_Write(lora_RegFrLsb, (byte)(Frequency_Output >> 0));
}



void check_LoRa()
{
Serial.print(F("Checking LoRa Device"));

  if (lora_CheckDevice() == true)
  {
    Serial.println(F(" - Present"));
    lora_Setup();                                      //Do the initial LoRa Setup
    lora_Print();
    lora_SetFreq(Start_Frequency, 0);
    Serial.print(F("Transmit FM Tone"));
    digitalWrite(LED1, HIGH);
    lora_Tone(1000, 2500, 2);                          //Transmit an FM tone, 1000hz, 2500ms, 2dBm
    digitalWrite(LED1, LOW);
    Serial.println(F(" - Done"));
    Serial.println();
    lora_Print();
  }
  else
  {
    Serial.println(F(" - LoRa Device Not Found"));
    lora_Print();
    Serial.println();
    led_Flash(100,25);
  }
 
  SPI.end();
  Serial.println();
  Serial.println();
  delay(1500);
}



void led_Flash(unsigned int flashes, unsigned int delaymS)
{
  //flash LED to show tracker is alive
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
  pinMode(LED1, OUTPUT); 			               //for PCB LED
  pinMode(lora_TonePin, INPUT_PULLUP);		   //ensure tone out pin is input
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

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  led_Flash(5,100);
 
}
