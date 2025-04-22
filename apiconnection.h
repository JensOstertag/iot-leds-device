#include <memory>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

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

JsonDocument fetchHttps(String endpoint,String payload) {
  HTTPClient httpClient;
  WiFiClientSecure wifiClient;
  wifiClient.setInsecure();
  JsonDocument emptyResponse;

  if(httpClient.begin(wifiClient, endpoint)) {
    // Send request
    int httpCode = httpClient.POST(payload);

    // Build response
    JsonDocument response;
    DeserializationError deserializationError = deserializeJson(response, httpClient.getString());

    if(deserializationError) {
      Serial.println("An error occurred whilst attempting to deserialize API response:");
      Serial.println(httpClient.getString());
      return emptyResponse;
    }

    return response;
  }

  return emptyResponse;
}

JsonDocument fetchHttp(String endpoint, String payload) {
  HTTPClient httpClient;
  WiFiClient wifiClient;
  JsonDocument emptyResponse;

  Serial.println("S1");

  if(httpClient.begin(wifiClient, endpoint)) {
    Serial.println("S2");
    // Send request
    int httpCode = httpClient.POST(payload);

    Serial.println("S3");

    // Build response
    JsonDocument response;
    DeserializationError deserializationError = deserializeJson(response, httpClient.getString());
    Serial.println("S4");

    if(deserializationError) {
      Serial.println("An error occurred whilst attempting to deserialize API response:");
      Serial.println(httpClient.getString());
      return emptyResponse;
    }

    Serial.println("S5");

    return response;
  }

  Serial.println("S6");

  return emptyResponse;
}
