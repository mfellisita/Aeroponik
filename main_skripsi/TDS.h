#ifndef TDS_H
#define TDS_H
#include <analogWrite.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "level.h"
#include "adc_lut.h"
#define TdsSensorPin 35
#define sensorSuhu 33
//GravityTDS gravityTds;
//Kalibrasi TDS
#define VREF 3.3              // analog reference voltage(Volt) of the ADC
const int NumReadings = 30;            // sum of sample point
int tdsReadings[NumReadings];       // Array untuk menyimpan pembacaan sensor TDS
float compensationVoltage;
float averageVoltage;
int medianTDS;

OneWire oneWire(sensorSuhu);
DallasTemperature sensors(&oneWire);
int pompaB = 26;
int pompaD = 14;
//float averageVoltage = 0;
int tdsValue = 0;
bool pompanutrisi = false;
float nilaiTDS, tdsVoltage, Suhu;
int tds;
bool tombol = true;

// Konfigurasi PID
const float kpTDS = 10;
const float kiTDS = 0.341;
const float kdTDS = 0;
int setpointTDS;
const float outputMinTDS = 0.0;
const float outputMaxTDS = 255.0;
// Variabel PID
float inputTDS, outputTDS, errorTDS;
float integralTDS, derivativeTDS, lastErrorTDS;

int adcRead(int _pin){
  int _raw = analogRead(_pin);
  int _result = (int)ADC_LUT[_raw];
  return _result;
}
void setupTDS(){
  pinMode(TdsSensorPin,INPUT);
  pinMode(pompaB, OUTPUT);
  pinMode(pompaD, OUTPUT);
  analogWrite(pompaB, 0);
  analogWrite(pompaD, 0);
  analogReadResolution(12);
}

int getMedian(int* values, int count) {
  // Mengurutkan array secara ascending
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (values[j] > values[j + 1]) {
        // Tukar posisi elemen
        int temp = values[j];
        values[j] = values[j + 1];
        values[j + 1] = temp;
      }
    }
  }

  // Mencari nilai median
  if (count % 2 == 0) {
    // Jika jumlah elemen genap, ambil rata-rata dari dua nilai tengah
    int middleIndex = count / 2;
    int median = (values[middleIndex - 1] + values[middleIndex]) / 2;
    return median;
  } else {
    // Jika jumlah elemen ganjil, ambil nilai tengah
    int middleIndex = count / 2;
    int median = values[middleIndex];
    return median;
  }
}

void cekSuhu(){
  sensors.requestTemperatures();
  Suhu = sensors.getTempCByIndex(0);
}

void cekTDS(){
    for (int i = 0; i < NumReadings; i++) {
      tdsReadings[i] = adcRead(TdsSensorPin);
      delay(10); // Delay untuk stabilisasi pembacaan
    }
    // Menghitung nilai median dari pembacaan sensor TDS
    medianTDS = getMedian(tdsReadings, NumReadings);
    nilaiTDS = medianTDS;
    averageVoltage = nilaiTDS * (float)VREF / 4096.0;
    float compensationCoefficient = 1.0+0.02*(Suhu-25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
    compensationVoltage=averageVoltage/compensationCoefficient; //temperature compensation
    tdsValue=(140.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 983.35*compensationVoltage)*0.8;//convert voltage value to tds value
    if(tdsValue < 0){
      tdsValue=0;
    }
}

int getTDS() {
	return tdsValue;
}

void prosesPIDTds(){
  errorTDS = setpointTDS - tdsValue;
  integralTDS += (errorTDS * kiTDS);
  if (integralTDS > outputMaxTDS){
    integralTDS = outputMaxTDS;}
  else if (integralTDS < outputMinTDS){
    integralTDS = outputMinTDS;}
  derivativeTDS = (errorTDS - lastErrorTDS) * kdTDS;
  outputTDS = (errorTDS * kpTDS) + integralTDS + derivativeTDS;
  if (outputTDS > outputMaxTDS){
    outputTDS = outputMaxTDS;}
  else if (outputTDS < outputMinTDS){
    outputTDS = outputMinTDS;}

  analogWrite(pompaB, outputTDS);
  analogWrite(pompaD, outputTDS);
  if (outputTDS > 0){
    pompanutrisi = true;
  }
//  Serial.println("PID TDS JALAN");
  lastErrorTDS = errorTDS;
}


void loopTDS(){
	//proses yang di-loop di TDS
	cekSuhu();
	cekTDS();
  Serial.println(String(nilaiTDS) + " Tegangan TDS:" + String(compensationVoltage) + " TDS: " + String(tdsValue) + "/" + String(setpointTDS) + " |Output:" + String(outputTDS) + " |Suhu:" + String(Suhu));
//  Serial.print(String(nilaiTDS) + ";" + String(compensationVoltage) + ";" + String(tdsValue) + ";" + String(setpointTDS) + ";" + String(outputTDS) + ";" + String(Suhu) + ";");
}

#endif
