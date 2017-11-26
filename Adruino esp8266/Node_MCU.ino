#include <SoftwareSerial.h>
#define DEBUG true

int Led = 13;

SoftwareSerial esp8266(10, 11); //  10-RX, 11-TX
char x;
void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

}
void loop() {
  while (esp8266.available()) {
    IncommingChar(esp8266.read());
  }
  if (x) {
    String html = "<!DOCTYPE HTML> <html>";
    html += "<head></head>";
    html += "<body><form action=\"\" method=\"get\">";
    html +=  "<input type=\"radio\" name=\"LED\" value=\"ON\"> LED_ON";
    html +=  "<input type=\"radio\" name=\"LED\" value=\"OFF\"> LED_OFF<br>";
    html +=   "<input type=\"submit\" value=\"Submit\">";
    html +=  "</form>";
    html +=  "</body></html>";

    String cipsend = "AT+CIPSEND=0,";
    cipsend += html.length();
    cipsend += "\r\n";
    SendData(cipsend, 1000, DEBUG);
    SendData(html, 1000, DEBUG);
    SendData("AT+CIPCLOSE=0\r\n", 1000, DEBUG);
    x = 0;
  }
}


void IncommingChar(const byte InChar) {
  static char InLine[300];
  static unsigned int Position = 0;
  switch (InChar) {
    case '\r':
      break;
    case '\n':
      InLine[Position] = 0;
      ProcessCommand(String(InLine));
      Position = 0;
      break;
    default:
      InLine[Position++] = InChar;
  }
}
void ProcessCommand (String InLine) {
  Serial.println("InLine: " + InLine);
  if (InLine.startsWith("+IPD,")) {
    x = 1;
  }
  if (InLine.indexOf("OFF") != -1) {
    digitalWrite(Led, LOW);
  }
  if (InLine.indexOf("ON") != -1) {
    digitalWrite(Led, HIGH);
  }

}

String SendData(String cmd, const int times, boolean debug) {
  String str = "";
  esp8266.print(cmd);
  long int time = millis();
  while ((time + times) > millis()) {
    while (esp8266.available()) {
      char c = esp8266.read();
      str += c;
    }
  }
  if (debug) {
    Serial.print(str);
  }
  return str;
}



