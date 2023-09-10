#ifndef NUTRISI_H
#define NUTRISI_H
#include <Arduino.h>

int nutrisiA = 18;
int nutrisiB = 19;
int buttonState1 = 1;
int buttonState2 = 1;
String statusA;
String statusB;

void setupNutrisi(){
  pinMode(nutrisiA, INPUT_PULLUP);
  pinMode(nutrisiB, INPUT_PULLUP);
}

String getA(){
  return statusA;
}

String getB(){
  return statusB;
}
 
void loopNutrisi(){
  buttonState1 = digitalRead(nutrisiA);
  if (buttonState1 == HIGH) {
    statusA = "FILLED";
  } else {
    statusA = "LOW";
  }
  buttonState2 = digitalRead(nutrisiB);
  if (buttonState2 == HIGH) {
    statusB = "FILLED";
  } else {
    statusB = "LOW";
  }
//  Serial.println("Nutrisi A: " + String(statusA) + " | Nutrisi B: " + String(statusB));
}

#endif
