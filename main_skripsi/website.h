#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include "TDS.h"
#include "level.h"
#include "pH.h"
#include "nutrisi.h"

#define BLYNK_TEMPLATE_ID "TMPL6phmJCq7p"
#define BLYNK_TEMPLATE_NAME "Aeroponik"
#define BLYNK_AUTH_TOKEN "U0PsIYVZ4SQBmHwnWFUM-cGSac-Ohsno"

char auth[] = "U0PsIYVZ4SQBmHwnWFUM-cGSac-Ohsno";
char ssid[] = "Kangmas Andy";
char pass[] = "esandy74";


bool sistemStatus = false;
bool LCD = false;

BLYNK_WRITE(V4) {
  int setTDS = param.asInt();
  setpointTDS = setTDS;
}

BLYNK_WRITE(V5) {
  float setPH = param.asFloat();
  setpointPH = setPH;
}

BLYNK_WRITE(V0) {
  bool sistem = param.asInt();
  sistemStatus = sistem;
  Serial.println("Sistem:" + String(sistem));
}

BLYNK_WRITE(V8) {
  bool lcdStatus = param.asInt();
  LCD = lcdStatus;
}

void setupWebsite() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
 // Connect to Blynk server
  Blynk.begin(auth, ssid, pass);

  // Print Wi-Fi and Blynk server information
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connected to Blynk server");
}

 
void loopWebsite(){// Run Blynk tasks
  Blynk.run();
  // Get TDS value
  int tds = getTDS();
  float ph = getPH();
  int levelair = getLevel();
  String nutA = getA();
  String nutB = getB();

  // Send TDS value to Blynk
  Blynk.virtualWrite(V1, tds);
  Blynk.virtualWrite(V2, ph);
  Blynk.virtualWrite(V3, levelair);
  Blynk.virtualWrite(V6, nutA);
  Blynk.virtualWrite(V7, nutB);
  Blynk.virtualWrite(V9, setpointTDS);
  Blynk.virtualWrite(V10, setpointPH);
  Blynk.virtualWrite(V11, setpointLevel);
 
  if(LCD == true){
    digitalWrite(RelayC, relayON);
//    Serial.println("LCD On");
  } 
  else{
    digitalWrite(RelayC, relayOFF);
//    Serial.println("LCD Off");
  }
}
