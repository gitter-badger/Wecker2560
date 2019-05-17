#if defined(__AVR_ATmega2560__)

#include <LiquidCrystal.h>
#include <ThreeWire.h>
#include "read.h"

#include <RtcDS1302.h>

boolean displayAnTaste = false;
boolean displayAn = true;
long long t = 0;
int index = 0;
int selInc = 0;
int up_down_inc = 0;
int cancel_counter = 0;

enum{rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7, b=10};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ThreeWire threeWire(42,44,46);
RtcDS1302<ThreeWire> Rtc(threeWire);
RtcDateTime aktuell;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  analogWrite(b, 255);
}

void loop() {
  lcd_key = read_LCD_buttons();
  lcd.setCursor(0,0);
  aktuell = Rtc.GetDateTime();
  displayAnTaste = false;
  
  beleuchtungRegeln();

  steuerungRegeln();

  if(index == 0){
    aktuelleZeitAbrufen();
  }

  if(index == 1){
    wecker();  
  }
  
  delay(100);
    
}

void beleuchtungRegeln(){
  if(buttonPressed(lcd_key) && !displayAn){
    t = millis();
    analogWrite(b, 255);
    displayAnTaste = true;
    displayAn = true;
  }
  
  if(millis()-t >= 6000){
    analogWrite(b, 100);
    displayAn = false;
  }
  
  if(millis()-t >= 8000){
    analogWrite(b, 0);
    displayAn = false;
  }
}

void aktuelleZeitAbrufen(){
    static char* wochentage[] = {
          "So","Mo","Di","Mi","Do","Fr","Sa"
      };
  
    lcd.print(wochentage[aktuell.DayOfWeek()]);
    lcd.print(",");
    if(aktuell.Day() <= 9) lcd.print("0");
    lcd.print(aktuell.Day());
    lcd.print(".");
    if(aktuell.Month() <= 9) lcd.print("0");
    lcd.print(aktuell.Month());
    lcd.print(".");
    lcd.print(aktuell.Year()-2000);
    lcd.setCursor(0,1);
    if(aktuell.Hour() <= 9) lcd.print("0");
    lcd.print(aktuell.Hour());
    lcd.print(":");
    if(aktuell.Minute() <= 9) lcd.print("0");
    lcd.print(aktuell.Minute());
    lcd.print(":");
    if(aktuell.Second() <= 9) lcd.print("0");
    lcd.print(aktuell.Second());
}

void wecker(){
    static int h;
    static int m;
    static int s = 0;
  
    if(selInc == 0){
      lcd.print("Wecker");
      lcd.setCursor(0,1);
      lcd.print("nicht gestellt");
    }
    if(selInc == 1){
      h = aktuell.Hour()+up_down_inc;
      lcd.print("H: ");
      lcd.print(h);
    }

    if(selInc == 2){
       m = aktuell.Minute()+up_down_inc;
       lcd.print("Min: ");
       lcd.print(m);  
    }

    if(selInc == 3){
       s = up_down_inc;
       lcd.print("Sec: ");
       lcd.print(s);  
    }
}

void steuerungRegeln(){
  if(!displayAnTaste){
    t = millis();
    
    switch(index){
      case 0: steuerung0(); break;
      case 1: steuerung1(); break; 
      default: return;  
    }
  }
}

void steuerung0(){
  Serial.println("Steuerung 0!");
    switch(lcd_key){
      case btnRIGHT: index++; lcd.clear(); break;
      case btnLEFT: index--; lcd.clear(); break;
      default: return;    
    }
}

void steuerung1(){
  Serial.println("Steuerung 1!");
    switch(lcd_key){
      case btnRIGHT: if(selInc == 0 || cancel_counter == 2){
            index++;
            if(selInc != 0) cancel_counter = 0;
          }
          else cancel_counter++; 
          lcd.clear(); 
          break;
      case btnLEFT: if(selInc == 0 || cancel_counter == 2){
            index--;
            if(selInc != 0) cancel_counter = 0;
          }
          else cancel_counter++; 
          lcd.clear(); 
          break;
      case btnSELECT: selInc++; up_down_inc = 0; lcd.clear(); break;
      case btnUP: up_down_inc++; break;
      case btnDOWN: up_down_inc--; break;
      default: return;    
    }
}

#else
void setup(){}
void loop(){}
#endif
