## LoRaSpy

This is a repository for the LoRaTracker LoRa5 library. This library is used in some of the more recent LoRaTracker programs such as the logger and the simple link tester. 

The library file does contain commands for sending and receiving packets but the library can also be used to provide a series of reporting and inspection programs when other programs are in use. 

To use the Library you should copy the LoRa5.h file into the same directory as your programs .ino file. You then need to modify your program to tell the LoRa5 library which are the 3 main pins that are used to interface with the SPI based SX1272 or SX1276/8. These pins should defined at the beginning of your program, just before the include for the LoRa5 library thus;

\#define lora_NSS 10

\#define lora_NReset

\#define lora_DIO0

\#include "LoRa5.h"

The LoRa5 library uses the standard SPI library routines to access the LoRa device, so its assumed you have already included this library in your program.

If you then compile your program, with whichever LoRa library that you are using, you should at this point notice no difference, there should be no compile errors. 

If in the running or testing of your program you wanted to know the current LoRa settings you can call the functions that will print out to the serial monitor the current LoRa modem settings;

lora_PrintLoRaSettings();

The output shows the LoRa devices current settings;

Frequency 434.400Mhz
Bandwidth 62500
Spreading Factor 8
Coding Rate CR4/5
Optimisation Off
Sync Word 0x12

The above commands can also be pronted individually such as;

lora_PrintBandwidth();

Which prints the current LoRa bandwidth. 

There are routines for printing the receive (RX) and transmit (TX) buffers in the LoRa device FIFO. The routines for this use the appropriate registers in the LoRa device to extract the information for where a packet received appears in the FIFO. Its possible that depending on exactly where the function is called in your program you may not get the exact results you require. This is the function to print to serial monitor the RX FIFO;

lora_RXFIFOPrint(PrintType);  

Where PrintType = 0 for ASCII output (characters), 1 for Decimal numbers and 2 for Hexadecimal numbers. 

The equivalent function for printing the TX FIFO is;

lora_TXFIFOPrint(PrintType);


The library routines will operate with the SX1272 or SX1276/8 LoRa devices. The setup of lora_RegModemConfig1 and lora_RegModemConfig2 on the LoRa device is different between these versions of the LoRa device so there are different library routines for the SX1272 and SX1276\8.

The library defaults to SX1276\8 mode, to use the library for SX1272 make sure there is a define '#define UseSX1272' in the settings file before including the library.
 
LoRaWAN uses a different sync word (0x34) from the default (0x12) used by the LoRaTracker libraries. The library uses the default sync word, to use the LoRaWAN syncword make sure there is a define '#define UseLoRaWAN' in the settings file. 

The user of these programs is expected to have had a reasonable level of experience in using the Arduino environment. Using the programs does require the user to make configuration changes so they will need to be able to cope with complier errors due to issues that they may create


#### Stuart Robinson
#### www.LoRaTracker.uk
#### December 2018

  