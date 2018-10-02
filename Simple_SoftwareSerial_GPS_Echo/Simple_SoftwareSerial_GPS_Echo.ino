/*
*******************************************************************************************************************************
Simple_SoftwareSerial_GPS_Echo

Reads characters from a GPS using Software Serial and echoes them to the Arduino Serial Monitor at 115200 baud.   

Define the pins to be used for Software Serial and the baud rate the GPS is using (9600 baud is common) below; 
  
*******************************************************************************************************************************
*/

#define GPSTX A2                          //pin number for TX output - data from Arduino into GPS
#define GPSRX A3                          //pin number for RX input - To Arduino from GPS
#define GPSBaud 9600                     //GPS Baud rate
#define Serial_Monitor_Baud 115200       //this is baud rate used for the Arduino IDE Serial Monitor

#include <Arduino.h>
#include <SoftwareSerial.h>                     
SoftwareSerial GPSserial(GPSRX, GPSTX);  


void loop()                    
{
  while (GPSserial.available() > 0)
  Serial.write(GPSserial.read());
}


void setup()
{
 Serial.begin(Serial_Monitor_Baud);       //setup Serial console ouput
 GPSserial.begin(GPSBaud);                //start softserial for GPS at defined baud rate
}


