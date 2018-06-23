//TTN_Node_Test.ino

// MIT License
// https://github.com/gonzalocasas/arduino-uno-dragino-lorawan/blob/master/LICENSE
// Based on examples from https://github.com/matthijskooijman/arduino-lmic
// Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman

// Adaptions: Andreas Spiess
// Adaptions: Stuart Robinson, June 2018


/*
**************************************************************************************************

  LoRaTracker Programs for Arduino

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
  intended purpose and free from errors.

  This is a test program for a The Things Network (TTN) node. It uses ABP (activating a device by personalization)
  to connect to the TTN network. 
  
  It sends the text message defined in the message[] array initialisation line, for instance;
  uint8_t message[] = "LoRaTracker"; 
  
  The message is sent at a seconds interval defined by;
  unsigned int TX_INTERVAL = 60;

  You can change the spreading factor used between SF7 (fast) and SF12 by enabling\disabling these defines;

  #define UseSF12                                               
  //#define UseSF7

  You will need to add the correct NWKSKEY, APPSKEY and DEVADDR for you node in the program, find the lines below at the 
  top of the program;
  
  u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  u1_t APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  u4_t DEVADDR = 0x00000000;  

  You get these keys by logging into the TTN console with your TTN account.
  
  Diagnostic information such as the frequency, spreading factor, bandwith, coding rate and transmit power are printed after every transmission. 
  The time the program waits before actually sending a packet is shown. If this period is longer than the programmed interval then the program is, 
  taking account of the transmit time, extending the interval to enforce a 1% duty cycle. 

  Changes:
  220618 - Added print of LoRa settings after packet transmit
  220618 - Change static const u1_t NWKSKEY[16] to u1_t NWKSKEY[16] so that complies with ESP32 which does not have -fpermissive set
  220618 - Works with ESP32
  230618 - Works with ATMEGA1284P
  230618 - Works with ATMEGA2560
  230618 - Works with BBC micro:bit
  230618 - Works with Arduino DUE - mod to WInterrupts.c needed see; https://forum.arduino.cc/index.php?topic=262394.0 post #4
  
  To do:
  Save frame counter to FRAM for load at reset
  What is switch to 869.525Mz, SF9 @startup ?
  LMIC library calculating transmit power incorrectly, the intended 14dBm is sent as 16dBm.



******************************************************************************************************
*/

#include <lmic.h>
#include <hal/hal.h>
#include "LoRaTracker_Debug.h"

//you define the hardware pins used here, if a pin is unused then change pin number to LMIC_UNUSED_PIN
#define lora_NSS 10                                      //NSS for LoRa device
#define lora_NReset 9                                    //NRESET for LoRa device
#define lora_DIO0 2                                      //DIO0 for LoRa device
#define lora_DIO1 5                                      //DIO1 for LoRa device
#define lora_DIO2 6                                      //DIO2 for LoRa device
#define LED1 8                                           //Define a pin for an LED, put at -1 if no LED used


//you can alternativly define the hardware pins for a LoRaTracker board by defining its library file here and removing 
//the #defines for the pins above
//#define LoRa_Device_in_MB1                             //needed for LoRaTracker boards with Mikrobus sockets
//#include "LCD_Receiver_Board_Definitions.h"
//#include "ESP32_Mikrobus_Shield_Board_Definitions.h"


u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u1_t APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u4_t DEVADDR = 0x00000000;              

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
unsigned long msSent;
unsigned long msschedule;

osjob_t sendjob;
uint8_t message[] = "LoRaTracker";                          //Payload to send (uplink)
unsigned int TX_INTERVAL = 60;                              //Schedule TX every this many seconds (might become longer due to duty cycle limitations)


#define UseSF12                                             //to send with SF12 use this define
//#define UseSF7                                            //to send with SF7 use this define


const lmic_pinmap lmic_pins = {                             //Pin mapping structure
  .nss = lora_NSS,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = lora_NReset,
  .dio = {lora_DIO0, lora_DIO1, lora_DIO2},
};


void loop()
{
  os_runloop_once();
}


void onEvent (ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      break;
    case EV_RFU1:
      Serial.println(F("EV_RFU1"));
      break;
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE"));
      msschedule = TX_INTERVAL * 1000;
      msSent = (millis() - msSent);
      Serial.print(F("Send delay "));
      Serial.print(msSent);
      Serial.print(F("mS  (Schedule "));
      Serial.print(msschedule);
      Serial.print(F("mS)"));
      
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
      }
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    default:
      Serial.println(F("Unknown event"));
      break;
  }
}


void do_send(osjob_t* j)
{
  if (LMIC.opmode & OP_TXRXPEND) {                                                     //Check if there is not a current TX/RX job running
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else                                                                                 //Prepare upstream data transmission at the next possible time.
  {
    digitalWrite(LED1, HIGH);
    msSent = millis();
    LMIC_setTxData2(1, message, sizeof(message) - 1, 0);                               //Next TX is scheduled after TX_COMPLETE event.
    Serial.println();
    Serial.println(F("Sending uplink packet"));
    print_PayloadAscii(message, (sizeof(message) - 2));
    Serial.println();
    print_PayloadHEX(message, (sizeof(message) - 2));
    Serial.println();
    print_LMICframe_Header();
    Serial.println();
    print_Frame_Count();
    Serial.println();
    print_CurrentLoRaSettings();
    digitalWrite(LED1, LOW);
    Serial.println(F("Waiting for EV_TXCOMPLETE"));
  }
}


void print_PayloadAscii(uint8_t temp_buffer[], uint8_t len)
{
  uint8_t i;

  Serial.print(F("Payload Ascii "));

  for (i = 0; i <= len; i++)
  {
    Serial.write(temp_buffer[i]);
  }
}


void print_PayloadHEX(uint8_t temp_buffer[], uint8_t len)
{
  uint8_t i, j;
  Serial.print(F("Payload HEX "));
  for (i = 0; i <= len; i++)
  {
    j = temp_buffer[i];
    if (j < 0x10)
    {
      Serial.print(F("0"));
    }
    Serial.print(j, HEX);
    Serial.print(F(" "));
  }
}

void print_LMICframe_Header()
{
  uint8_t i, j;

  Serial.print(F("Frame Header "));

  for (i = 0; i <= 7; i++)
  {
    j = LMIC.frame[i];
    if (j < 0x10)
    {
      Serial.print(F("0"));
    }
    Serial.print(j, HEX);
    Serial.print(F(" "));
  }
}


void print_Frame_Count()
{
  uint8_t low, high;
  unsigned int frames;
  low = LMIC.frame[6];
  high = LMIC.frame[7];
  frames = (high * 256) + low;
  Serial.print(F("Frame Count "));
  Serial.print(frames);
}


void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting"));
  Serial.println(F("LoRaTracker TTN Node Test"));
  Serial.println();

  pinMode(LED1, OUTPUT);                                                              //for PCB LED

  os_init();                                                                          //LMIC init
  LMIC_reset();                                                                       //Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);                                   //Set static session parameters.
  LMIC_setLinkCheckMode(0);                                                           //Disable link check validation
  LMIC.dn2Dr = DR_SF9;                                                                //TTN uses SF9 for its RX2 window.

  lora_Print();
  Serial.println();

#ifdef UseSF12                                                                         //Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)  
  LMIC_setDrTxpow(DR_SF12, 10);
  Serial.println(F("Using SF12"));
#endif

#ifdef UseSF7                                                                          //Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library) 
  LMIC_setDrTxpow(DR_SF7, 10);
  Serial.println(F("Using SF7"));
#endif

#ifdef CFG_eu868
  Serial.println(F("CFG_eu868 set"));
#endif

#ifdef CFG_us915
  Serial.println(F("CFG_us915 set"));
#endif

#ifdef CFG_sx1276_radio
  Serial.println(F("CFG_sx1276_radio set"));
#endif

#ifdef CFG_sx1272_radio
  Serial.println(F("CFG_sx1272_radio set"));
#endif

  do_send(&sendjob);                                                                     //Start job
}


