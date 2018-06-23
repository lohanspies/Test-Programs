//LoRa_Test.h
/*
*******************************************************************************************************************************
  Easy Build LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 23/6/18

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  To Do:


*******************************************************************************************************************************
*/

//SX127x Register names
const byte lora_RegFifo = 0x00;
const byte lora_WRegFifo = 0x80;
const byte lora_RegOpMode = 0x01;
const byte lora_RegFdevLsb = 0x05;
const byte lora_RegFrMsb = 0x06;
const byte lora_RegFrMid = 0x07;
const byte lora_RegFrLsb = 0x08;
const byte lora_RegPaConfig = 0x09;
const byte lora_RegOcp = 0x0B;
const byte lora_RegLna = 0x0C;
const byte lora_RegFifoAddrPtr = 0x0D;
const byte lora_RegFifoTxBaseAddr = 0x0E;
const byte lora_RegFifoRxBaseAddr = 0x0F;
const byte lora_RegFifoRxCurrentAddr = 0x10;
const byte lora_RegIrqFlagsMask = 0x11;
const byte lora_RegIrqFlags = 0x12;
const byte lora_RegRxNbBytes = 0x13;
const byte lora_RegRxHeaderCntValueMsb = 0x14;
const byte lora_RegRxHeaderCntValueLsb = 0x15;
const byte lora_RegRxPacketCntValueMsb = 0x16;
const byte lora_RegRxPacketCntValueLsb = 0x17;
const byte lora_RegPktSnrValue = 0x19;
const byte lora_RegPktRssiValue = 0x1A;
const byte lora_RegRssiValue = 0x1B;
const byte lora_RegFsiMSB = 0x1D;
const byte lora_RegFsiLSB = 0x1E;
const byte lora_RegModemConfig1 = 0x1D;
const byte lora_RegModemConfig2 = 0x1E;
const byte lora_RegSymbTimeoutLsb = 0x1F;
const byte lora_RegPreambleLsb = 0x21;
const byte lora_RegPayloadLength = 0x22;
const byte lora_RegFifoRxByteAddr = 0x25;
const byte lora_RegModemConfig3 = 0x26;
const byte lora_RegFeiMsb = 0x28;
const byte lora_RegFeiMid = 0x29;
const byte lora_RegFeiLsb = 0x2A;
const byte lora_RegPacketConfig2 = 0x31;
const byte lora_RegDioMapping = 0x40;
const byte lora_RegPllHop = 0x44;

//Miscellaneous definitions
const byte LowDoptON = 0x08;    //value to turn low data rate optimization on
const byte LowDoptOFF = 0x00;   //value to turn low data rate optimization off

//precalculated values for frequency error calculation
//const PROGMEM  float bandwidth[] = {0.1309, 0.1745, 0.2617, 0.3490, 0.5234, 0.6996, 1.049, 2.097, 4.194, 8.389}; 


void writeReg (u1_t addr, u1_t data ) {
    hal_pin_nss(0);
    hal_spi(addr | 0x80);
    hal_spi(data);
    hal_pin_nss(1);
}

u1_t readReg (u1_t addr) {
    hal_pin_nss(0);
    hal_spi(addr & 0x7F);
    u1_t val = hal_spi(0x00);
    hal_pin_nss(1);
    return val;
}


float lora_GetFreq()
{
  //get the current set LoRa frequency
  byte lora_LFMsb, lora_LFMid, lora_LFLsb;
  unsigned long lora_Ltemp;
  float lora_Ltemp1;
  lora_LFMsb = readReg(lora_RegFrMsb);
  lora_LFMid = readReg(lora_RegFrMid);
  lora_LFLsb = readReg(lora_RegFrLsb);
  lora_Ltemp = ((lora_LFMsb * 0x10000ul) + (lora_LFMid * 0x100ul) + lora_LFLsb);
  lora_Ltemp1 = ((lora_Ltemp * 61.03515625) / 1000000ul);
  return lora_Ltemp1;
}


unsigned long lora_returnbandwidth(byte BWregvalue)
{
  switch (BWregvalue)
  {
    case 0:
      return 7800;

    case 16:
      return 10400;

    case 32:
      return 15600;

    case 48:
      return 20800;

    case 64:
      return 31200;

    case 80:
      return 41700;

    case 96:
      return 62500;

    case 112:
      return 125000;

    case 128:
      return 250000;

    case 144:
      return 500000;

    default:
      break;
  }
  return 0;
}




void print_CurrentLoRaSettings()
{
  //prints the current LoRa settings, reads device registers
  float tempfloat;
  //uint32_t tempint;
  byte regdata;
  byte power;
  unsigned long bw;

  tempfloat = lora_GetFreq();
  Serial.print(tempfloat,3);
  Serial.print(F("MHz"));

  regdata = readReg(lora_RegModemConfig1);
  regdata = regdata & 0xF0;
  bw = lora_returnbandwidth(regdata);
  Serial.print(F("  BW"));
  Serial.print(bw);

  regdata = readReg(lora_RegModemConfig2);
  regdata = (regdata & 0xF0) / 16;
  Serial.print(F("  SF"));
  Serial.print(regdata);

  regdata = readReg(lora_RegModemConfig1);
  regdata = regdata & B00001110;
  regdata = regdata / 2; //move right one
  regdata = regdata + 4;

  Serial.print(F("  CR4/"));
  Serial.print(regdata);

  regdata = readReg(lora_RegModemConfig3);
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

  regdata = readReg(lora_RegPaConfig);
  
  #ifdef LORADEBUG
  Serial.print(F("  lora_RegPaConfig = "));
  Serial.print(regdata);
  #endif

  if (regdata & 0x80)                          //if PAselect is set
  {
  Serial.print(F("  PABoost"));
  power = regdata & 0x0f;                      //mask off bits 7-4  
  power = 17 - (15 - power);                   //formulae from datasheet   
  Serial.print(F("  Power "));
  Serial.print(power);
  Serial.print(F("dBm"));
  Serial.println();
  }
  else
  {
  Serial.print(F("  RFO Power ?"));
  } 

}


void lora_Print()
{
  //prints the contents of LoRa registers to serial monitor
  byte Loopv1, Loopv2, Reg, RegData;

  Serial.println(F("LoRa Registers"));
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
      RegData = readReg(Reg);
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

