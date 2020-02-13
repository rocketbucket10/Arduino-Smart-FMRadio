#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RDA5807M.h>
#include "DS3231.h"
#define OLED_RESET 4
#define FIX_BAND RADIO_BAND_FM

int setButton = 10;
int backButton = 9;
int functionSwitch = 8;
int whereami;
int functionSwitchButton;

Adafruit_SSD1306 display(OLED_RESET);
RDA5807M radio;
RTClib RTC;
int stations[12] = {9110,9190,9270,9350,9430,9830,10010,10280,10400,10480,10640,10710};

void setup() {
  Serial.begin(9600);
  
  pinMode(setButton, INPUT);
  pinMode(backButton, INPUT);
  pinMode(functionSwitch, INPUT);
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  radio.init();
  radio.setBandFrequency(FIX_BAND, 9110);
  radio.setVolume(6);
  radio.setMono(true);
  radio.setMute(false);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,10);
  display.print(stations[whereami] / 100.0);
  display.display();
}

void loop() {
  Serial.print(digitalRead(functionSwitch));
  Serial.print(digitalRead(backButton));
  Serial.println(digitalRead(setButton));

  display.clearDisplay();
  display.setCursor(20,10);
  
   if (functionSwitchButton == 0) { //FM rádió
     if (digitalRead(setButton)== HIGH){
      if (whereami == 11) whereami = 0;
      else whereami++;
      setFrequency();
    }

    if (digitalRead(backButton)== HIGH){
      if (whereami == 0) whereami = 11;
      else whereami--;
      setFrequency();
    }
  } else if (functionSwitchButton == 1) { //MP3
      mp3Start();
  } else { //Óra és pára
      oraPara();
  }
  
  if (digitalRead(functionSwitch) == HIGH){
    if (functionSwitchButton == 2){
      functionSwitchButton = 0;
    }
    else{
      functionSwitchButton++;
    }
    
  }

  while (digitalRead(setButton) == HIGH || digitalRead(backButton) == HIGH || digitalRead(functionSwitch) == HIGH);
}

void setFrequency(){
  float displayStation;
  displayStation = stations[whereami]/100.0;
  display.print(displayStation);
  display.display();
  radio.setBandFrequency(FIX_BAND, stations[whereami]);
}
void mp3Start(){

  display.print("MP3");
  display.display();
}
void oraPara(){
    display.setCursor(10,7);
    DateTime now = RTC.now();
    display.print(now.year());
    display.print("/");
    display.print(now.month());
    display.print("/");
    display.print(now.day());
    display.print(" ");
    display.print(now.hour());
    display.print(":");
    display.print(now.minute());
    display.print(":");
    display.print(now.second());
    display.display();
}
