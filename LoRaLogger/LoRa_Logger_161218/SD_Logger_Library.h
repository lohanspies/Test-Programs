//SD_Logger_Library.h
/*
*****************************************************************************************************************************
  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 12/12/2018

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit
  permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.
*****************************************************************************************************************************
*/

#ifdef USESD

void SD_PrintHEXByte0x(uint8_t temp)
{
  logFile.print(F("0x"));
  if (temp < 0x10)
  {
    logFile.print(F("0"));
  }
  logFile.print(temp, HEX);
}

void SD_PrintHEXByte(uint8_t temp)
{
  if (temp < 0x10)
  {
    logFile.print(F("0"));
  }
  logFile.print(temp, HEX);
}

void SD_PrintOptimisation()
{
  boolean temp;
  temp = lora_returnSetOptimisation();
  if (temp)
  {
    logFile.print(F("On"));
  }
  else
  {
    logFile.print(F("Off"));
  }
}

void SD_PrintCodingRate()
{
  logFile.print(lora_returnSetCodingRate());
}


void SD_PrintBandwidth()
{
  logFile.print(lora_returnSetBandwidth());
}


void SD_PrintSpreadingFactor()
{
  logFile.print(lora_returnSetSpreadingFactor());
}


void SD_PrintFrequency()
{
  float tempfrequency;
  tempfrequency = lora_GetFreqFloat();
  logFile.print(tempfrequency, 3);
}


void SD_PrintSyncWord()
{
  uint8_t temp;
  temp = lora_Read(lora_RegSyncWord);                    //Read the syncword
  SD_PrintHEXByte0x(temp);
}


void SD_PrintSyncWord0x()
{
  uint8_t temp;
  temp = lora_Read(lora_RegSyncWord);                    //Read the syncword
  logFile.print(F("0x"));
  SD_PrintHEXByte(temp);
}


void SD_Print()
{
  //prints the contents of LoRa registers to serial monitor
  uint8_t Loopv1, Loopv2, Reg, RegData;

  logFile.print(F("Reg    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F"));
  logFile.println();
  Reg = 0;
  for (Loopv1 = 0; Loopv1 <= 7; Loopv1++)
  {
    logFile.print(F("0x"));
    logFile.print(Loopv1, HEX);              //print the register number
    logFile.print(F("0  "));
    for (Loopv2 = 0; Loopv2 <= 15; Loopv2++)
    {
      RegData = lora_Read(Reg);
      if (RegData < 0x10) {
        logFile.print(F("0"));
      }
      logFile.print(RegData, HEX);           //print the register number
      logFile.print(F(" "));
      Reg++;
    }
    logFile.println();
  }
}


void SD_PrintASCIIorHEX(uint8_t temp)
{
  //logFile.println(F("lora_PrintASCIIorHEX()"));
  if ( (temp < 0x10) || (temp > 0x7E) )
  {
    logFile.print(F(" ("));
    SD_PrintHEXByte(temp);
    logFile.print(F(") "));
  }
  else
  {
    logFile.write(temp);
  }
}

void SD_RXBuffPrint(uint8_t PrintType)
{
  //Print contents of RX buffer as ASCII,decimal or HEX
  //PrintType = 0 = ASCII
  //PrintType = 1 = Decimal
  //PrintType = 2 = HEX
  uint8_t bufferData;

  if (PrintType == 0)
  {
    SD_PrintASCIIorHEX(lora_RXPacketType);
    SD_PrintASCIIorHEX(lora_RXDestination);
    SD_PrintASCIIorHEX(lora_RXSource);
  }

  if (PrintType == 1)
  {
    SD_PrintHEXByte(lora_RXPacketType);
    logFile.print(F(" "));
    SD_PrintHEXByte(lora_RXDestination);
    logFile.print(F(" "));
    SD_PrintHEXByte(lora_RXSource);
    logFile.print(F(" "));
  }


  if (PrintType == 2)
  {
    logFile.print(lora_RXPacketType);
    logFile.print(F(" "));
    logFile.print(lora_RXDestination);
    logFile.print(F(" "));
    logFile.print(lora_RXSource);
    logFile.print(F(" "));
  }



  for (uint8_t index = lora_RXStart; index <= lora_RXEnd; index++)
  {
    if (PrintType == 0)
    {
      SD_PrintASCIIorHEX(lora_RXBUFF[index]);
    }

    if (PrintType == 1)
    {
      //lora_TXBUFF[index])
      SD_PrintHEXByte(lora_RXBUFF[index]);
      logFile.print(F(" "));
    }

    if (PrintType == 2)
    {
      bufferData = lora_RXBUFF[index];
      if (bufferData < 0x10)
      {
        logFile.print(F("0"));
      }
      logFile.print(bufferData, HEX);
      logFile.print(F(" "));
    }
  }
}


void SD_FIFOPrint(uint8_t PrintType, uint8_t FIFOstart, uint8_t FIFOlength)
{
  //Print contents of FIFO as ASCII,decimal or HEX
  //PrintType = 0 = ASCII, 1 = Decimal, 2 = HEX
  //If ASCII is selcted, not pintable characters are converted to (00) style hex

  uint8_t FIFOData, index, FIFOend;

  FIFOend = FIFOstart + FIFOlength - 1;

  for (index = FIFOstart; index <= FIFOend; index++)
  {

    lora_Write(lora_RegFifoAddrPtr, index);          //set FIFO ptr to index
    
    digitalWrite(lora_NSS, LOW);                       
    SPI.transfer(lora_RegFifo);
    FIFOData = SPI.transfer(0);                       
    digitalWrite(lora_NSS, HIGH);                    //finish the byte read 

    if (PrintType == 0)
    {
      SD_PrintASCIIorHEX(FIFOData);
    }

    if (PrintType == 1)
    {
      SD_PrintHEXByte(FIFOData);
      logFile.print(F(" "));
    }

    if (PrintType == 2)
    {
      if (FIFOData < 0x10)
      {
        logFile.print(F("0"));
      }
      logFile.print(FIFOData, HEX);
      logFile.print(F(" "));
    }
  }
  digitalWrite(lora_NSS, HIGH);
}


void SD_RXFIFOPrint(uint8_t PrintType)
{
  uint8_t FIFOlength, FIFOstart;
  FIFOlength = lora_Read(lora_RegRxNbBytes);
  FIFOstart = lora_Read(lora_RegFifoRxBaseAddr);
  SD_FIFOPrint(PrintType, FIFOstart, FIFOlength);
}


void SD_TXFIFOPrint(uint8_t PrintType)
{
  //assume lora_RegFifoAddrPtr is last byte of TX packet written to FIFOuint8_t FIFOlength, FIFOstart;
  uint8_t FIFOlength, FIFOstart;
  FIFOlength = lora_Read(lora_RegFifoAddrPtr) - lora_Read(lora_RegFifoTxBaseAddr) + 1;
  FIFOstart = lora_Read(lora_RegFifoTxBaseAddr);
  SD_FIFOPrint(PrintType, FIFOstart, FIFOlength);
}


void SD_PrintLoRaSettings()
{
  //uint8_t temp;
  //float tempfloat;
  logFile.print(F("Frequency "));
  SD_PrintFrequency();
  logFile.println(F("Mhz"));

  logFile.print(F("Bandwidth "));
  SD_PrintBandwidth();
  logFile.println();

  logFile.print(F("Spreading Factor "));
  SD_PrintSpreadingFactor();
  logFile.println();

  logFile.print(F("Coding Rate CR4/"));
  SD_PrintCodingRate();
  logFile.println();

  logFile.print(F("Optimisation "));
  SD_PrintOptimisation();
  logFile.println();
   
  logFile.print(F("Sync Word "));
  SD_PrintSyncWord();
  logFile.println();
}


void SD_PrintReceptionInfo()
{
  //print the information for packet last received
  //note, lora_PacketSNR has already been converted into a signed value
  //lora_PacketRSSI is a signed value also

  logFile.print(F("SNR,"));
  logFile.print(lora_PacketSNR);
  logFile.print(F("dB"));

  logFile.print(F("  RSSI,"));
  logFile.print(lora_PacketRSSI);
  logFile.print(F("dB "));

  logFile.print(F("  Length,"));
  logFile.print(lora_Read(lora_RegRxNbBytes));

}


void SD_PrintHeader()
{
  logFile.println(__FILE__);
  logFile.print(F("Compiled "));
  logFile.print(__TIME__);
  logFile.print(F(" "));
  logFile.println(__DATE__);
  logFile.println(F(authorname));
  logFile.println();
}


void SD_printUptime()
{
  uint32_t uptime;
  uptime = millis() / 1000;
  logFile.print(uptime);
  logFile.print(F(" Seconds"));
}


void SD_PrintTotals()
{
  logFile.print(F("OK Packets "));
  logFile.print(lora_PacketOKCount);
  logFile.print(F("  CRC Errors "));
  logFile.print(lora_RXCRCerrorCount);
  logFile.print(F("  Header Errors "));
  logFile.print(lora_RXHeadererrorCount);
  logFile.print(F("  Phantom Packets "));
  logFile.println(lora_PhantomPacketCount);
}


uint8_t setup_SDLOG()
{
  //checks if the SD card is present and can be initialised
  //returns log number, 1-99, if OK, 0 if not
  uint8_t i;

  Serial.print(F("SD card..."));

  if (!SD.begin(SD_CS))
  {
    Serial.println(F("ERROR SD card fail"));
    Serial.println();
    SD_Found = false;
    return 0;
  }

  Serial.print(F("Initialized OK - "));
  SD_Found = true;

  for (i = 1; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logFile = SD.open(filename, FILE_WRITE);
      break;                                      
    }
  }

  Serial.print(F("Writing to "));
  Serial.println(filename);

  SD_PrintHeader();
  SD_PrintLoRaSettings();
  return i;
}

#endif

