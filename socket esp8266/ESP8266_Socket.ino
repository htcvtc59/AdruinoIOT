#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <SoftwareSerial.h>

extern "C" {
#include "user_interface.h"
}

const byte RX = D1;
const byte TX = D2;

SoftwareSerial mySerial = SoftwareSerial(RX, TX, false, 256);

SocketIOClient client;

const char* ssid = "Ngoc Thao";
const char* password = "01672639857";

char host[] =  "iotappweb.herokuapp.com"; //iotappweb.herokuapp.com
int httpPort = 80; //80

extern String RID;
extern String Rfull;

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.println("Connected to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
}

void loop() {
  if (client.monitor()) {
    mySerial.print(RID);
    mySerial.print('\r');
    mySerial.print(Rfull);
    mySerial.print('\r');

    Serial.print(RID);
    Serial.print(' ');
    Serial.println(Rfull);

    uint32_t free = system_get_free_heap_size();
    Serial.println(free);

  }
  if (!client.connected()) {
    client.reconnect(host, httpPort);
  }

}
