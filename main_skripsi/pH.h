#ifndef PH_H
#define PH_H
#include <analogWrite.h>
#include <Wire.h>
#include "level.h"
#define phPin 32

//kalibrasi PH
#define offsetPH 18.34            //deviation compensate
int numReadings = 100; // Jumlah pembacaan untuk diambil nilai tertinggi
float highestValue = 0.0; // Nilai pH tertinggi
float highestPH;

float PhValue = 0;
float PH_step;
float nilaiPH;
float TeganganPh;
bool pompaPH = false;

// Inisialisasi pin untuk motor pH Up dan pH Down
int pompaUpPin = 25; //pH UP > Pompa A
int pompaDownPin = 27; //pH DOWN > Pompa C

// Konfigurasi PID
const float kpPH = 5;        // Konstanta proporsional
const float kiPH = 0.5;        // Konstanta integral
const float kdPH = 0;        // Konstanta derivatif
// Konfigurasi setpointPH dan batas output
float setpointPH;
const float outputMinPH = 0.0; // Batas minimum output motor
const float outputMaxPH = 255.0; // Batas maksimum output motor
// Variabel PID
float inputPH, outputPH, errorPH;
float integralPH, derivativePH, lastErrorPH;

void setupPh(){
  pinMode(pompaUpPin, OUTPUT);
  pinMode(pompaDownPin, OUTPUT);
  pinMode(phPin, INPUT);
}

float cekPh(){
    nilaiPH = analogRead(phPin);
    TeganganPh = nilaiPH * 3.3 / 4096.0;
    PhValue = -5.70*TeganganPh+offsetPH;
    if(PhValue < 0){
      PhValue = 0;
      }
    if(PhValue > 14.00){
      PhValue = 14.00;
      }
}

float readHighestPH() {
  highestValue = 0;
  float hh=0;
  for (int aa = 0; aa<20; aa++){
    for (int i = 0; i < numReadings; i++) {
      float pHValue = cekPh();
      if (pHValue > highestValue) {
        highestValue = pHValue;
      }
      delay(1); // Jeda antara pembacaan pH
    }
    hh=highestValue+hh;
  }
  highestValue=hh/20;
  return highestValue;
}

float getPH() {
  return highestPH;
}
void kontrolPompa(float outputPH) {
  if (outputPH > 0) {
    // Hidupkan pompa turun pH
    analogWrite(pompaUpPin, 0);
    analogWrite(pompaDownPin, outputPH);
    Serial.print("Pompa Down ON");
  } else if (outputPH < 0) {
    // Hidupkan pompa naik pH
    analogWrite(pompaDownPin, 0);
    analogWrite(pompaUpPin, -outputPH);
    Serial.print("Pompa Up ON");
  } else {
    // Matikan kedua pompa
    digitalWrite(pompaDownPin, LOW);
    digitalWrite(pompaUpPin, LOW);
  }
}

void prosesPIDpH(){
    float currentPH = PhValue;
    float errorPH = setpointPH - currentPH;
    integralPH += errorPH * kiPH;
    float derivativePH = (errorPH - lastErrorPH);
    outputPH = kpPH * errorPH + integralPH + kdPH * derivativePH;
    if (outputPH > 255.0) {
      outputPH = 255.0;
    }
    if(outputPH != 0){
      pompaPH = true;
    }
    kontrolPompa(outputPH);
    lastErrorPH = errorPH;
}


void loopPh(){
	cekPh();
	highestPH = readHighestPH();
  Serial.println(String(nilaiPH) + " Tegangan PH:" + String(TeganganPh) + " PH: " + String(highestPH) + "/" + String(setpointPH) + " | Output:" + String(outputPH));
//  Serial.println(String(nilaiPH) + ";" + String(TeganganPh) + ";" + String(highestPH) + ";" + String(setpointPH) + ";" + String(outputPH));
}

#endif
