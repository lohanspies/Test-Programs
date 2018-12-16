## LoRaLogger

This is a repository for the LoRaTracker LoRa logger program

The purpose of the programs is to log received LoRa Packets to the Serial monitor or attached SD card. The program has options for the SX1272 or SX1276/8 and default or LoRaWAN mode

### Program operation

This program will receive a raw LoRa packet based on the LoRa settings in the 'Settings.h' file. The SNR, RSSI and packet length will be displayed on the serial monitor, together with the ASCII of HEX contents of the  packet. Any ASCII control characters less then 0x20 are replaced with HEX values in brackets. The packet data can also be logged to an attached SD card as text or hexadecimal, the format is the same as the serial monitor output.

Depending on the time gaps between packets you may need a faster processor than an 8Mhz Arduino Pro Mini to ensure that a packet is printed and logged before the next one arrives.

You need to specify the pins that connect to the LoRa device and SD card select in the 'Settings.h' file

The program will operate with the SX1272 or SX1276/8 devices. The setup of lora_RegModemConfig1 and lora_RegModemConfig2 on the LoRa device is different between these versions of the LoRa device so there are different library routines for the SX1272 and SX1276\8.

The library defaults to SX1276\8 mode, to use the library for SX1272 make sure there is a define '#define UseSX1272' in the settings file before including the library.
 
LoRaWAN uses a different sync word (0x34) from the default (0x12) used by the LoRaTracker libraries. The library uses the default sync word, to use the LoRaWAN syncword make sure there is a define '#define UseLoRaWAN' in the settings file. 

Do not forget to fit an antenna to the LoRa device, you can destroy it if you use it without and antenna.



#### Stuart Robinson
#### www.LoRaTracker.uk
#### December 2018

  