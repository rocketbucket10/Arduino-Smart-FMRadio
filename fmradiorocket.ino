#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RDA5807M.h>
#include <BME280I2C.h>
#include "DS3231.h"
#define FIX_BAND     RADIO_BAND_FM
#define OLED_RESET 4
byte setButton = 10;
byte backButton = 9;
byte functionSwitch = 8;
byte whereami;
byte functionSwitchButton; 
BME280I2C bme;
Adafruit_SSD1306 display(OLED_RESET);
RDA5807M radio;
RTClib RTC;
int stations[8] = {8830,9070,9270,9630,9710,9830,10160,10300};

void setup() {
  Serial.begin(9600);
  pinMode(setButton, INPUT);
  pinMode(backButton, INPUT);
  pinMode(functionSwitch, INPUT);
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  radio.init();
  radio.setBandFrequency(FIX_BAND, 8830);
  radio.setVolume(6);
  radio.setMono(false);
  radio.setMute(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(stations[whereami] / 100.0);
  display.display();
}

void loop() {
  display.setCursor(0,10);
  display.clearDisplay();
   if (functionSwitchButton == 0) { //FM rádió
     if (digitalRead(setButton)== HIGH){
      if (whereami == 7) whereami = 0;
      else whereami++;
      setFrequency();
    }

    if (digitalRead(backButton)== HIGH){
      if (whereami == 0) whereami = 7;
      else whereami--;
      setFrequency();
    }
  } else if (functionSwitchButton == 1) { //homerseklet
      homerseklet(); //homerseklet
  } else { //Óra
      ora();
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
void homerseklet(){
   bme.begin();
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);
   display.print("H: ");
   display.print(temp);
   display.print("C");
   display.print(" ");
   display.print(" ");
   display.print("P: ");
   display.print(hum);
   display.print("%");
   display.print(" ");
   display.print("NY: ");
   display.print(pres / 1000);
   display.print("kPa");
   display.display();
}
void ora(){
  
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
