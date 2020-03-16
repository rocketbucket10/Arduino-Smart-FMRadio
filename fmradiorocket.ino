#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RDA5807M.h>
#include <BME280I2C.h>
#include "DS3231.h"
#define FIX_BAND     RADIO_BAND_FM
#define OLED_RESET 4
byte setButton = 8;
byte backButton = 10;
byte functionSwitch = 9;
byte whereami;
byte functionSwitchButton; 
BME280I2C bme;
Adafruit_SSD1306 display(OLED_RESET);
RDA5807M radio;
RTClib RTC;
int stations[8] = {8830,9070,9270,9630,9710,9830,10160,10300};
String stationname[8]  = {"Danko", "Bartok", "Petofi", "Radio 1", "Kossuth", "Retro", "Radio M", "Csillag"};

void setup() {
  Serial.begin(9600);
  pinMode(setButton, INPUT);
  pinMode(backButton, INPUT);
  pinMode(functionSwitch, INPUT);
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;);
  }
  display.display();
  display.clearDisplay();
  radio.init();
  radio.setBandFrequency(FIX_BAND, 8830);
  radio.setVolume(15);
  radio.setMono(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.print(stations[whereami] / 100.0);
  display.println(" Danko");
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
      display.clearDisplay();
      display.setCursor(0,10);
      setFrequency();
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
  display.print(" ");
  display.print(stationname[whereami]);
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
   display.print(" ");
   display.print(" ");
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
    if(now.month()<10){
      display.print("0");
    }
    display.print(now.month());
    display.print("/");
    display.print(now.day());
    display.print(" ");
    if(now.hour()<10){
      display.print("0");
    }
    display.print(now.hour(), DEC);
    display.print(":");
    if(now.minute()<10){
      display.print("0");
    }
    display.print(now.minute(), DEC);
    display.print(":");
    if(now.second()<10){
      display.print("0");
    }
    display.print(now.second(), DEC);
    display.display();
}
