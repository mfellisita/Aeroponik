#include <LiquidCrystal_I2C.h>
#include "TDS.h"
#include "pH.h"
#include "level.h"
#include "website.h"
#include "nutrisi.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);
unsigned long tdscounter, tdsdelay, phcounter, phdelay;
bool enabletds = true;
bool enableph = true;

void setup() {
  Serial.begin(115200);
  lcd.init();                      
  lcd.backlight();
  setupTDS();
  setupPh();
  setupLevel();
  setupNutrisi();
  digitalWrite(RelayC, relayON);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("Network...");
  setupWebsite();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Aero");
  lcd.setCursor(0, 1);
  lcd.print("Check Blynk");
  delay(5000);
  lcd.clear();
}

void loop(){
  loopWebsite();
  loopNutrisi();
  loopLevel();
  loopTDS();
  loopPh();
  if(sistemStatus == true){
   valveAir();
   if (Level > (setpointLevel-5)) {
      if (enabletds == true ){
//        Serial.print("TDS Adjusment active");
        prosesPIDTds();
      }
      //timer pompa nutrisi
      if (pompanutrisi == true){
        tdscounter ++;
        if (tdscounter == 10){
          //matikan pompa nutrisi
          enabletds = false;
          analogWrite(pompaB, 0);
          analogWrite(pompaD, 0);
          pompanutrisi = false;
          tdsdelay = 0;
        }
      }
      if (tdscounter >= 10 && pompanutrisi == false){
        tdsdelay ++;
        Serial.println("TDS Wait");
        if (tdsdelay == 10){
          tdscounter = 0;
          enabletds = true;
        }
      }
   }
// PH ADJUSTMENT**********************************************
    if (Level > (setpointLevel-5)) {
      if (enableph == true ){
//        Serial.print("PH Adjusment active");
        prosesPIDpH();
      }
//      timer pompa nutrisi
      if (pompaPH == true){
        phcounter ++;
        if (phcounter == 10){
          //matikan pompa PH
          enableph = false;
          analogWrite(pompaUpPin, 0);
          analogWrite(pompaDownPin, 0);
          pompaPH = false;
          phdelay = 0;
        }
      }
      if (phcounter >= 30 && pompaPH == false){
        phdelay ++;
        Serial.println("PH Wait");
        if (phdelay == 30){
          phcounter = 0;
          enableph = true;
        }
      }
//       Serial.println("Pompa status:\t enable\t counter1\t counter2");
//       Serial.println("TDS:" + String(pompanutrisi) + "\t" + String(enabletds) + "\t" + String(tdscounter) + "\t" + String(tdsdelay));
//       Serial.println("PH:" + String(pompaPH) + "\t" + String(enableph) + "\t" + String(phcounter) + "\t" + String(phdelay));
    } 
  }
  else{
    analogWrite(pompaUpPin, 0);
    analogWrite(pompaB, 0);
    analogWrite(pompaDownPin, 0);
    analogWrite(pompaD, 0);
  }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TDS:");
    lcd.print(tdsValue);
    lcd.setCursor(8,0);
    lcd.print("/");
    lcd.print(setpointTDS);
    lcd.setCursor(0, 1);
    lcd.print("pH:");
    lcd.print(highestPH);
    lcd.setCursor(8,1);
    lcd.print("/");
    lcd.print(setpointPH);
    lcd.setCursor(12,1);
    lcd.print(" ");
    lcd.setCursor(13,1);
    lcd.print(Level);
    lcd.setCursor(15,1);
    lcd.print("%");
    delay(100);
}
