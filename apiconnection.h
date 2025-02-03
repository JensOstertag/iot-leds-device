#include <memory>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

void setupWiFiConnection() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.print("Connected - Local IP address: ");
  Serial.println(WiFi.localIP());
}

void handleHttpConnection(HTTPClient *httpClient) {
  // Build POST payload
  JsonDocument post;
  post["deviceUid"] = DEV_UID;
  post["deviceApiKey"] = DEV_API_KEY;
  char postJson[512];

  // Send request
  int httpCode = httpClient->POST(postJson);
  Serial.println(httpCode);

  Serial.println(httpClient->getString());

  // Build response payload
  JsonDocument response;
  deserializeJson(response, httpClient->getString());

  // TODO: Parse animation
}

void fetch() {
  HTTPClient httpClient;

  if(String(WEB_HOST).startsWith("https")) {
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();

    if(httpClient.begin(wifiClient, String(WEB_HOST) + "/api/get-device-animation")) {
      handleHttpConnection(&httpClient);
    } else {
      Serial.println("failed");
    }
  } else {
    WiFiClient wifiClient;

    if(httpClient.begin(wifiClient, String(WEB_HOST) + "/api/get-device-animation")) {
      handleHttpConnection(&httpClient);
    } else {
      Serial.println("failed");
    }
  }

  httpClient.end();
}
