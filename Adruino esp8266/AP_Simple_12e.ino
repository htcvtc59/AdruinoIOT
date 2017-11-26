#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "Ngoc Thao";
const char WiFiPSK[] = "01672639857";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN[] = {16, 5, 4}; // Thing's onboard, green LED

WiFiServer server(80);

void setupWiFi()
{
  byte ledStatus = LOW;
  Serial.println();
  Serial.println("Connecting to: " + String(WiFiSSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFiSSID, WiFiPSK);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN[0], ledStatus);
    digitalWrite(LED_PIN[1], ledStatus);
    digitalWrite(LED_PIN[2], ledStatus);
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    delay(100);
  }
  Serial.println("WiFi connected: ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_PIN[0], LOW);
  digitalWrite(LED_PIN[1], LOW);
  digitalWrite(LED_PIN[2], LOW);
}
void setupMDNS() {
  if (!MDNS.begin("thing")) {
    Serial.println("Error setting up MDNS ");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("DNS started");
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(LED_PIN[0], OUTPUT);
  pinMode(LED_PIN[1], OUTPUT);
  pinMode(LED_PIN[2], OUTPUT);
  digitalWrite(LED_PIN[0], LOW);
  digitalWrite(LED_PIN[1], LOW);
  digitalWrite(LED_PIN[2], LOW);

}

void setup()
{
  initHardware();
  setupWiFi();
  server.begin();
  setupMDNS();
}

void loop()
{
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

 
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  
  int val = -1; 
 
  if (req.indexOf("/led/0") != -1)
    val = 0;
  else if (req.indexOf("/led/1") != -1)
    val = 1;
  else if (req.indexOf("/led/2") != -1)
    val = 2;
  else if (req.indexOf("/led/3") != -1)
    val = 3;
  else if (req.indexOf("/led/4") != -1)
    val = 4;
  else if (req.indexOf("/led/5") != -1)
    val = 5;
  else if (req.indexOf("/read") != -1)
    val = -2;

  
  if (val == 0)
    digitalWrite(LED_PIN[0], HIGH);
  else if (val == 1)
    digitalWrite(LED_PIN[1], HIGH);
  else if (val == 2)
    digitalWrite(LED_PIN[2], HIGH);
  else if (val == 3)
    digitalWrite(LED_PIN[0], LOW);
  else if (val == 4)
    digitalWrite(LED_PIN[1], LOW);
  else if (val == 5)
    digitalWrite(LED_PIN[2], LOW);

  client.flush();

  
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  
  if (val >= 0)
  {
    s += "LED is now ";
    s += (val == 3 || val == 4 || val == 5) ? "off" : "on";
  }
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</html>\n";

  client.print(s);
  
  delay(1);
  Serial.println("Client disonnected");

}



