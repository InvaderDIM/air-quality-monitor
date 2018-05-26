//******************************
//*Collecting data about dust in an air from PMS5003 and displaying it in LCD
//******************************
 
#include <Arduino.h>
#include "SPI.h"
#include <Adafruit_ILI9341.h>

#define TFT_DC 2
#define TFT_CS  5

#define LEN 31             //0x42 + 31 bytes. Message length from dust sensor
unsigned char buf[LEN];

// Main dust sensor parameters
int pm1=0;               //Number on particles less that 1.0 ug/m3
int pm2_5=0;              //Number on particles less that 2.5 ug/m3
int pm10=0;               //Number on particles less that 10.0 ug/m3

int prev_pm1=0;
int prev_pm2_5=0;
int prev_pm10=0;

// Detailed dust sensor parameters
int dust0_3=0;           //Number on particles bigger that 0.3 ug/m3
int dust0_5=0;
int dust1=0;
int dust2_5=0;
int dust5=0;
int dust10=0;

int prev_dust0_3=0;
int prev_dust0_5=0;
int prev_dust1=0;
int prev_dust2_5=0;
int prev_dust5=0;
int prev_dust10=0;


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


void setup()
{
  //Configuring display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);

  tft.println("PM1.0:      ug/m3");            
  tft.println("PM2.5:      ug/m3");     
  tft.println("PM10:       ug/m3");   

  tft.println("");  
  tft.setTextSize(2);
  tft.println("0.3<     0.5<     1<");
  tft.println("2.5<      5<     10<");
  
  Serial.begin(9600);   
  Serial.setTimeout(1500);
}

void loop()
{
 
}
