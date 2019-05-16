#include <LiquidCrystal.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include "read.h"



enum{rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  analogWrite(10, 0);
  lcd.print("hey");

}

void loop() {
  lcd_key = read_LCD_buttons();
  if(buttonPressed(lcd_key)){
    long long int t = millis();
    analogWrite(10, 200);
    while(millis()-t < 2000);
    analogWrite(10, 0);  
  }
}
