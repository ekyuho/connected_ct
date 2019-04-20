#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "EmonLib.h"
EnergyMonitor emon1;
unsigned long mark, s = 0;

void delivering(String payload) {
  WiFiClient client;
  HTTPClient http;
  Serial.println("Sent: " + payload);
  if (http.begin(client, "http://t.damoa.io:8080/add?u=402999&f=3&s=" + String(s++) + "&i=0E" + payload)) {
    int code = http.GET();
    if (code == HTTP_CODE_OK) Serial.println(http.getString());
    else Serial.println(http.errorToString(code));
    http.end();
  } else
    Serial.println("failed to connect");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nBegin");
  WiFi.begin("cookie", "0317137263");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());
  emon1.current(A0, 111.1);
  mark = millis() + 60000;
}


void loop() {
  if (millis() > mark ) {
    mark = millis() + 60000;
    delivering(String(emon1.calcIrms(1480)));
  }
}
