#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>  
const byte RX = 3;          // Chân 3 được dùng làm chân RX
const byte TX = 2;          // Chân 2 được dùng làm chân TX
 
SoftwareSerial mySerial = SoftwareSerial(RX, TX); 
 
 
SerialCommand sCmd(mySerial); 
 
int red = 4, blue = 5; 
 
void setup() {
  Serial.begin(57600);
 
  
  mySerial.begin(57600);
  
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  
  
  
  sCmd.addCommand("LED",   led); 
  Serial.println("Cmd Ready");
}
 
void loop() {
  sCmd.readSerial();
}
 
void led() {
  Serial.println("LED");
  char *json = sCmd.next(); 
  Serial.println(json);
  StaticJsonBuffer<200> jsonBuffer; 
  JsonObject& root = jsonBuffer.parseObject(json);
 
  int redStatus = root["led"][0];
  int blueStatus = root["led"][1];
 
  Serial.print(F("redStatus "));
  Serial.println(redStatus);
  Serial.print(F("blueStatus "));
  Serial.println(blueStatus);
 
 
  digitalWrite(red, redStatus);
  digitalWrite(blue, blueStatus);
}
