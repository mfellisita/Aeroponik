#ifndef LEVEL_H
#define LEVEL_H
const int RelayA = 4;
const int RelayB = 0;
const int RelayC = 2;
const int RelayD = 15;
int levelPin = 34;
float sensorlevel;
int setpointLevel = 50;
int Level;
int mapLevel;
const int sensorMin = 0;
const int sensorMax = 1000;
const int outputMin = 0;
const int outputMax = 100;
int relayON = LOW; //relay nyala
int relayOFF = HIGH; //relay mati
signed int gapLevel;


void setupLevel() {
  pinMode(levelPin, INPUT);
  pinMode(RelayB, OUTPUT);
  pinMode(RelayC, OUTPUT);
  digitalWrite(RelayB, relayOFF);
}

void cekLevel(){
  sensorlevel = analogRead(levelPin)-100;  //Sensor is having some offset so do some calibration here
  if (sensorlevel <= 0){
  sensorlevel = 0;
  }
  mapLevel = map(sensorlevel, sensorMin, sensorMax, outputMin, outputMax);
  Level = constrain(mapLevel, 0, 100);
}

void valveAir(){
  gapLevel = setpointLevel - Level;
  if(gapLevel > 3){
    digitalWrite(RelayB, relayON);
//    Serial.println("Valve ON");
    }
  else{
    digitalWrite(RelayB, relayOFF);
//    Serial.println("Valve OFF");
  }
}

int getLevel(){
  return Level;
}

void loopLevel() {
  cekLevel();
  Serial.println("Raw Level: " + String(sensorlevel) + " | Level(%): " + String(Level) + "/" + String(setpointLevel) + " |GapLevel: " + String(gapLevel));
// Serial.print(String(sensorlevel) + ";" + String(Level) + ";" + String(setpointLevel) + ";");
}

#endif
