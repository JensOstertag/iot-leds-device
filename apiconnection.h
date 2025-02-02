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

void fetch() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  HTTPClient httpClient;
  if(httpClient.begin(*client, String(WEB_HOST) + "/api/get-device-animation")) {
    JsonDocument post;
    post["deviceUid"] = DEV_UID;
    post["deviceApiKey"] = DEV_API_KEY;
    char postJson[512];
    int httpCode = httpClient.POST(postJson);

    Serial.println(httpCode);

    String payload = httpClient.getString();
    Serial.println(payload);
  } else {
    Serial.println("failed");
  }
  httpClient.end();
}
