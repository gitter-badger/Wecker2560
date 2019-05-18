#if defined(__AVR_ATmega2560__)

#include <LiquidCrystal.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "read.h"


boolean displayAnTaste = false;
boolean displayAn = true;
boolean alarmAn = false;
unsigned long t = 0;
auto index = 0;
auto selIndex = 0;
auto up_down_inc = 0;

enum{rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7, b=10};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ThreeWire threeWire(42,44,46);
RtcDS1302<ThreeWire> Rtc(threeWire);
RtcDateTime aktuell;
RtcDateTime alarmZeit;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  analogWrite(b, 255);
  Rtc.Begin();
}

void loop() {
  lcd_key = read_LCD_buttons();
  lcd.setCursor(0,0);
  aktuell = Rtc.GetDateTime();
  displayAnTaste = false;
  
  beleuchtungRegeln();
  steuerungRegeln();
  alarmRegeln();

  if(index == 0){
    zeitDarstellen(aktuell);
  }

  if(index == 1){
    wecker();  
  }
  Serial.println(int(t));
    
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

void zeitDarstellen(RtcDateTime zeit){
    static char* wochentage[] = {
          "So","Mo","Di","Mi","Do","Fr","Sa"
      };
    lcd.print(wochentage[zeit.DayOfWeek()]);
    lcd.print(",");
    if(zeit.Day() <= 9) lcd.print("0");
    lcd.print(zeit.Day());
    lcd.print(".");
    if(zeit.Month() <= 9) lcd.print("0");
    lcd.print(zeit.Month());
    lcd.print(".");
    lcd.print(zeit.Year()-2000);
    lcd.setCursor(0,1);
    if(zeit.Hour() <= 9) lcd.print("0");
    lcd.print(zeit.Hour());
    lcd.print(":");
    if(zeit.Minute() <= 9) lcd.print("0");
    lcd.print(zeit.Minute());
    lcd.print(":");
    if(zeit.Second() <= 9) lcd.print("0");
    lcd.print(zeit.Second());
}

void zeitDarstellen(RtcDateTime zeit, int x, int y, boolean wochentag){
    static char* wochentage[] = {
          "So","Mo","Di","Mi","Do","Fr","Sa"
      };

    lcd.setCursor(x,y);
    if(wochentag){
    lcd.print(wochentage[zeit.DayOfWeek()]);
    lcd.print(",");}
    if(zeit.Day() <= 9) lcd.print("0");
    lcd.print(zeit.Day());
    lcd.print(".");
    if(zeit.Month() <= 9) lcd.print("0");
    lcd.print(zeit.Month());
    lcd.print(".");
    lcd.print(zeit.Year()-2000);
    lcd.setCursor(0,1);
    if(zeit.Hour() <= 9) lcd.print("0");
    lcd.print(zeit.Hour());
    lcd.print(":");
    if(zeit.Minute() <= 9) lcd.print("0");
    lcd.print(zeit.Minute());
    lcd.print(":");
    if(zeit.Second() <= 9) lcd.print("0");
    lcd.print(zeit.Second());
}

void alarmRegeln(){
    if(alarmAn){
    if(aktuell.TotalSeconds() >= alarmZeit.TotalSeconds()){
            lcd.clear();
            selIndex = 5;
            alarm();
      }
    }  
}

void alarm(){
  unsigned long t2 = millis();
  tone(38, 500);
  warten(t2);
  noTone(38);
  t2 = millis();
  warten(t2);
  if(alarmAn) alarm();
}

void warten(unsigned long t2){
    while(millis()-t2 < 2000){
    lcd_key = read_LCD_buttons();
    if(buttonPressed(lcd_key)) {alarmAn = false;Serial.println("ausgeschaltet");}
  }
}

void wecker(){
    static int h;
    static int m;
    static int s = 0;


    if(selIndex == 0){
      lcd.print("Wecker");
      lcd.setCursor(0,1);
      lcd.print("nicht gestellt");
      return;
    }
    
    if(selIndex == 1){
      h = aktuell.Hour()+up_down_inc;
      lcd.print("H: ");
      if(h <= 9) lcd.print("0");
      lcd.print(h);
      return;
    }

    if(selIndex == 2){
       m = aktuell.Minute()+up_down_inc;
       lcd.print("Min: ");
       if(m <= 9) lcd.print("0");
       lcd.print(m);  
    }

    if(selIndex == 3){
       s = up_down_inc;
       lcd.print("Sec: ");
       if(s <= 9) lcd.print("0");
       lcd.print(s);
       return;  
    }

    if(selIndex == 4){
      alarmZeit = RtcDateTime(aktuell.Year(), 
      aktuell.Month(), aktuell.Day(), h, m, s);
      alarmAn = true;
      lcd.print("Alarm: ");
      zeitDarstellen(alarmZeit, 7, 0, false); 
    }

    if(selIndex == 5){
      lcd.print("Alarm ausgel\357st");
      lcd.setCursor(0,1);
      lcd.print("Guten ");
      int alarm_h = alarmZeit.Hour();
      if(alarm_h < 5){
          lcd.setCursor(0,4);
          lcd.print(" Nacht");
        }
      if(alarm_h >= 5 && alarm_h < 12){
          lcd.print(" Morgen");
        }
        
      if(alarm_h >= 12 && alarm_h < 18){
          lcd.print(" Tag");
        }

      if(alarm_h >= 18 && alarm_h < 24){
          lcd.print(" Abend");
        }
    }
}

void steuerung0(){
    switch(lcd_key){
      case btnRIGHT: index++; lcd.clear(); break;
      case btnLEFT: index--; lcd.clear(); break;
      default: return;    
    }
}

void steuerung1(){
    switch(lcd_key){
      case btnSELECT: if(selIndex < 4){selIndex++; 
      lcd.clear(); 
      up_down_inc = 0;}
      break; 
      case btnRIGHT: if(selIndex > 0) selIndex = 0; else {index++;lcd.clear();} break;
      case btnLEFT: if(selIndex > 0) selIndex = 0; else {index--;lcd.clear();} break; 
      case btnUP: up_down_inc++; break;
      case btnDOWN: up_down_inc--; break;
      default: return;    
    }
}

void steuerung2(){
  Serial.println("Steuerung 2!");
  switch(lcd_key){
      case btnRIGHT: index++; lcd.clear(); break;
      case btnLEFT: index--; lcd.clear(); break;
      default: return;    
    } 
}

void steuerungRegeln(){
  if(!displayAnTaste){
    if(buttonPressed(lcd_key)) t = millis(); 
    switch(index){
      case 0: steuerung0(); break;
      case 1: steuerung1(); break;
      case 2: steuerung2(); break; 
      default: return;  
    }
  }
}

#else
void setup(){}
void loop(){}
#endif
