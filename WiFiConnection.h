#include <ESP8266WiFi.h>

void setupWiFiConnection() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected - Local IP address: ");
  Serial.println(WiFi.localIP());
}
