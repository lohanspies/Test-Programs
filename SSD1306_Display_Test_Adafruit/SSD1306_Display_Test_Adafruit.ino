                                                                //SSD1306_Display_Test_Adafruit.ino
/*
**************************************************************************************************

  Easy Build LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 05/10/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  This test program uses the Adafruit SSD1306 driver library; //https://github.com/adafruit/Adafruit_SSD1306

  For a 128 x 64 OLED.
 **************************************************************************************************

*/


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>                           //https://github.com/adafruit/Adafruit_SSD1306
unsigned int count = 0;
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

void loop() 
{
  Serial.println(F("Running Display Test"));
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setCursor(0,12);
  display.println("LoRaTracker");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  delay(1000);
}


void setup() 
{
  Serial.begin(38400);
  Serial.print("SD1306 Display Test");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);           //initialize with the I2C addr 0x3C (for the 128x64)
}





