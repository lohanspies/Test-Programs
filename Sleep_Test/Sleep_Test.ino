#define programname "Sleep_Test"
#define programversion "V1.0"
#define dateproduced "09/08/2018"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>


/*
*****************************************************************************************************************************
LoRaTracker Test Programs

Copyright of the author Stuart Robinson - 09/08/2018

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

The program flashes a LED connected to the pin defined by LED1, then puts the processor to sleep for 8 seconds after which 
there should be a watchdog wakeup. The 'Reset' message on the console should only be seen after reset or power up. When the processor wakes up after sleep it
should resume running the program at the point it went sleep, this is noted on the Serial Monitor by the 'Wake up' message.

The program is useful for checking the sleep current. Due to varaitions in the accuracy of the watchdog timer on the Atmel
processor, the sleep time might not always be 8 seconds.  


********************************************************************************************************************************
*/

#define LED1 13                            //Arduino pin number for LED, when high LED should be on.


#include <LowPower.h>                      //https://github.com/rocketscream/Low-Power


#define Serial_Monitor_Baud 115200         //this is baud rate used for the Arduino IDE Serial Monitor


void loop() 
{
  Serial.println(F("LED Flash"));
  led_Flash(25,50);
  sleep8secs(1);                           //sleep for 1 count of 8 seconds 
  Serial.println(F("Wake up"));
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


void sleep8secs(unsigned int LNumberSleeps)
{
  //note sleep time may not be an accurate 8 seconds due to device tolerances
  unsigned int i;

  Serial.print(F("zz "));
  Serial.println(LNumberSleeps);
  Serial.flush();                                      //let print complete

  for (i = 1; i <= LNumberSleeps; i++)
  {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);    //sleep 1 second
  }
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);          //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
  pinMode(LED1, OUTPUT);                     //for PCB LED
  Serial.println(F("Reset"));
 }



