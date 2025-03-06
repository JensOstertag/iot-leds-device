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

void handleHttpConnection(HTTPClient *httpClient) {
  // Build POST payload
  JsonDocument postData;
  postData["deviceUid"] = DEV_UID;
  postData["deviceApiKey"] = DEV_API_KEY;

  String requestPayload;
  serializeJson(postData, requestPayload);

  // Send request
  int httpCode = httpClient->POST(requestPayload);

  // Build response
  JsonDocument response;
  DeserializationError deserializationError = deserializeJson(response, httpClient->getString());

  if(deserializationError) {
    Serial.println("An error occurred whilst attempting to deserialize API response:");
    Serial.println(httpClient->getString());
    return;
  }

  if(response["code"] != 200) {
    Serial.print("An error occurred whilst attempting to fetch the API. Received error code ");
    Serial.print(response["code"].as<int>());
    Serial.print(" (");
    Serial.print(response["message"].as<const char*>());
    Serial.println(").");
    return;
  }

  // Parse animation
  Animation animation = parseAnimation(response["data"]["animation"]);
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
