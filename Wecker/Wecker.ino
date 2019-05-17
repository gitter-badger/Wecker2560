#include <LiquidCrystal.h>
#include <ThreeWire.h>
#include "read.h"

#include <RtcDS1302.h>




enum{rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7, b=10};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long long t = 0;
ThreeWire myWire(42,44,46);
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  analogWrite(b, 200);
}

void loop() {
  handleBacklight();
  if(millis()-t >= 10000) analogWrite(b, 0);

  handleCurrentDateTime();
    
}

void handleBacklight(){
  lcd_key = read_LCD_buttons();
  if(buttonPressed(lcd_key)){
    t = millis();
    analogWrite(b, 200);
  }
}

void handleCurrentDateTime(){
    RtcDateTime current = Rtc.GetDateTime();
    //lcd.print();
}
