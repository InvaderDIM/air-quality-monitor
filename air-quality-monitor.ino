//******************************************************************************************
//*Collecting data about dust in an air from PMS5003 and displaying it in LCD
//******************************************************************************************
 
#include <Arduino.h>
#include "SPI.h"
#include <Adafruit_ILI9341.h>

#define TFT_DC 2
#define TFT_CS  5

#define LEN 31             //0x42 + 31 bytes. Message length from dust sensor
unsigned char buf[LEN];

typedef struct {
   int value;
   int oldValue;
   int displayXCoord;      //X coordinate on display
   int displayYCoord;      //Y coordinate on display
} co2;

typedef struct {
  char name[];
  int value;
  int oldValue;
  int bytePos;            //Strat byte in message for that value
  int displayXCoord;      //X coordinate on display
  int displayYCoord;      //Y coordinate on display
} dust;

// Main dust sensor parameters
dust mainDustMetrics[] = {
  { "PM1.0",  0, 0, 9, 120, 0 },          //Number on particles less that 1.0 ug/m3
  { "PM2.5",  0, 0, 11, 120, 24 },         //Number on particles less that 2.5 ug/m3
  { "PM10.0", 0, 0, 13, 120, 48 }          //Number on particles less that 10.0 ug/m3
};

// Detailed dust sensor parameters
dust detailedDustMetrics[] = {
  { "PM0.3",  0, 0, 15, 50, 96 },         //Number on particles bigger that 0.3 ug/m3
  { "PM0.5",  0, 0, 17, 155, 96 },
  { "PM1.0",  0, 0, 19, 243, 96 },
  { "PM2.5",  0, 0, 21, 50, 112 },
  { "PM5.0",  0, 0, 23, 145, 112 },
  { "PM10.0", 0, 0, 25, 243, 112 }
};

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


void setup() {
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

void loop() {
   if(Serial.find(0x42)) {    //Dust sensor message stsrts with 0x42
    Serial.readBytes(buf,LEN);
  
    if(buf[0] == 0x4d){
      if(checkMessage(buf,LEN)){
        //Parse ad display main dust metrics   
        for (int i=0; i<sizeof(mainDustMetrics); i++) {
          mainDustMetrics[i].value = getDustValue(buf, mainDustMetrics[i]);
          if (mainDustMetrics[i].oldValue != mainDustMetrics[i].value) {  
            displayMetrics(mainDustMetrics[i], 3);
            mainDustMetrics[i].oldValue = mainDustMetrics[i].value;
          }
        }
        //Parse ad display detailed dust metrics   
        for (int i=0; i<sizeof(detailedDustMetrics); i++) {
          detailedDustMetrics[i].value = getDustValue(buf, detailedDustMetrics[i]); 
          if (detailedDustMetrics[i].oldValue != detailedDustMetrics[i].value) {  
            displayMetrics(detailedDustMetrics[i], 2);
            detailedDustMetrics[i].oldValue = detailedDustMetrics[i].value;
          }
        }
        
        consoleOutput();
      }           
    } 
    
  }
}

char checkMessage(unsigned char *buf, char len) { 
  bool messageIsOK=false;
  int checkSum=0;

  for(int i=0; i<(len-2); i++) {
  checkSum=checkSum+buf[i];
  }
  checkSum=checkSum + 0x42;
 
  if(checkSum == ((buf[len-2]<<8)+buf[len-1])) {
    messageIsOK = true;
  }
  
  return messageIsOK;
}

int getDustValue(unsigned char *buf, dust metric) {
  return (buf[metric.bytePos]<<8) + buf[metric.bytePos+1];
}

void displayMetrics(dust metric, int textSize) {
  int width = 80;
  int height = 21;
  if (textSize == 2) {
      width=50;
      height=14;
  }
  tft.setTextColor(ILI9341_GREEN);
  tft.fillRect(metric.displayXCoord, metric.displayYCoord, width, height, ILI9341_BLACK);
  tft.setCursor(metric.displayXCoord, metric.displayYCoord);
  tft.println(metric.value);
}


void consoleOutput() {
    Serial.println((buf[1]<<8) + buf[2]); 
    
    Serial.print((buf[3]<<8) + buf[4]);    Serial.print("  ");
    Serial.print((buf[5]<<8) + buf[6]);    Serial.print("  ");
    Serial.println((buf[7]<<8) + buf[8]); 
    
    Serial.print((buf[9]<<8) + buf[10]);   Serial.print("  ");
    Serial.print((buf[11]<<8) + buf[12]);  Serial.print("  ");
    Serial.println((buf[13]<<8) + buf[14]);
    
    Serial.print((buf[15]<<8) + buf[16]);  Serial.print("  ");
    Serial.print((buf[17]<<8) + buf[18]);  Serial.print("  ");
    Serial.print((buf[19]<<8) + buf[20]);  Serial.print("  ");
    Serial.print((buf[21]<<8) + buf[22]);  Serial.print("  ");
    Serial.print((buf[23]<<8) + buf[24]);  Serial.print("  ");
    Serial.println((buf[25]<<8) + buf[26]);
    

    Serial.println("");
    Serial.println("");  
}

