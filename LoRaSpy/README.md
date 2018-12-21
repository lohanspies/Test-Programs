## LoRaSpy

This is a repository for the LoRaTracker LoRa5 Arduino library. This library is used in some of the more recent LoRaTracker programs such as the logger and the simple link tester. 

The library file does contain commands for sending and receiving packets but the library can also be used to provide a series of reporting and inspection functions when other programs or libraries are in use. 

To use the Library you should copy the LoRa5.h file into the same directory as your programs .ino file. You then need to modify your program to tell the LoRa5 library which are the 3 main pins that are used to interface with the SPI based SX1272 or SX1276/8. These pins should defined at the beginning of your program, just before the include for the LoRa5 library thus;

\#define lora_NSS 10

\#define lora_NReset 9

\#define lora_DIO0 2

\#include "LoRa5.h" 

The LoRa5 library uses the standard SPI library routines (#include <SPI.h> ) to access the LoRa device, so its assumed you have already included and initialised this library in your your program and that its tested and working.

The lora\_NSS pin is used by the LoRa5 library to as the select access the LoRa device, so it must be defined. The lora_NReset pin is used by some of the Lora5.h device initialisation functions and if your not going to use them, you do not need to use this pin. You should then define it as unused;

\#define lora_DIO0 -1 

If you make the changes above then compile your program, with whichever LoRa library that you are using, you should at this point notice no difference, there should hopefully be no compile errors. 

If in the running or testing of your program you wanted to know the current LoRa settings you can call the functions that will print out to the serial monitor the current LoRa modem settings;

lora_PrintLoRaSettings();

The output shows the LoRa devices current settings;

Frequency 434.400Mhz
Bandwidth 62500
Spreading Factor 8
Coding Rate CR4/5
Optimisation Off
Sync Word 0x12

The above commands can also be printed individually such as;

lora_PrintBandwidth();

Which prints the current LoRa bandwidth. 

There are routines for printing the receive (RX) and transmit (TX) buffers in the LoRa device FIFO. The routines for this use the appropriate registers in the LoRa device to extract the information for where a packet received appears in the FIFO. Its possible that depending on exactly where the function is called in your program you may not get the exact results you require. This is the function to print to serial monitor the RX FIFO;

lora_RXFIFOPrint(PrintType);  

Where PrintType = 0 for ASCII output (characters), 1 for Decimal numbers and 2 for Hexadecimal numbers. 

The equivalent function for printing the TX FIFO is;

lora_TXFIFOPrint(PrintType);

One useful routine is to be able to print the current registers on the LoRa device;

lora_Print();

This produces a output similar to below;

    Reg    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    0x00  50 8D 1A 0B 00 52 6C 99 99 4F 09 2B 23 01 80 00 
    0x10  00 00 00 00 00 00 00 00 04 00 00 40 00 62 87 FF 
    0x20  00 0C 01 FF 00 00 00 00 00 00 00 00 0E 50 14 45 
    0x30  55 C3 05 27 1C 0A 03 0A 42 34 49 1D 00 AF 00 00 
    0x40  00 00 12 24 2D 00 03 00 04 23 00 09 05 84 32 2B 
    0x50  14 00 00 11 00 00 00 0F E0 00 0C 04 07 00 5C 78 
    0x60  00 19 0C 4B CC 0E 41 20 04 47 AF 3F BA 00 02 0B 
    0x70  D0 01 10 00 00 00 00 00 00 00 00 00 00 00 00 00 


The library routines will operate with the SX1272 or SX1276/8 LoRa devices. The setup of lora_RegModemConfig1 and lora_RegModemConfig2 on the LoRa device is different between these versions of the LoRa device so there are different library routines for the SX1272 and SX1276\8.

The library defaults to SX1276\8 mode, to use the library for SX1272 make sure there is a define '#define UseSX1272' in the settings file before including the library.
 
LoRaWAN uses a different sync word (0x34) from the default (0x12) used by the LoRaTracker libraries. The library uses the default sync word, to use the LoRaWAN syncword make sure there is a define '#define UseLoRaWAN' in the settings file. 

The user of these programs is expected to have had a reasonable level of experience in using the Arduino environment. Using the programs does require the user to make configuration changes so they will need to be able to cope with complier errors due to issues that they may create


#### Stuart Robinson
#### www.LoRaTracker.uk
#### December 2018

  