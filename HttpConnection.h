#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

void fetchHttps(String endpoint, String payload) {
  JsonDocument emptyResponse;

  WiFiClientSecure client;
  client.setInsecure();

  if(!client.connect(WEB_HOST, WEB_PORT)) {
    Serial.println("Could not connect to remote host");
    return;
  }

  String request = "";
  request += "POST " + endpoint + " HTTP/1.1\r\n";
  request += "Host: " + String(WEB_HOST) + "\r\n";
  request += "User-Agent: ESP8266/IotLed\r\n";
  request += "Connection: close\r\n";
  request += "Content-Type: application/json\r\n";
  request += "Content-Length: " + String(payload.length()) + "\r\n";
  request += "\r\n";
  request += payload;

  client.println(request);

  unsigned long startRequest = millis();
  while(client.available() == 0) {
    yield();
    if(millis() - startRequest > 5000) {
      Serial.println("Could not connect to remote host (timeout)");
      client.stop();
      break;
    }
  }
  
  client.stop();
}

void fetchHttp(String endpoint, String payload) {
  WiFiClient client;

  if(!client.connect(WEB_HOST, WEB_PORT)) {
    Serial.println("Could not connect to remote host");
    return;
  }

  String request = "";
  request += "POST " + endpoint + " HTTP/1.1\r\n";
  request += "Host: " + String(WEB_HOST) + "\r\n";
  request += "User-Agent: ESP8266/IotLed\r\n";
  request += "Connection: close\r\n";
  request += "Content-Type: application/json\r\n";
  request += "Content-Length: " + String(payload.length()) + "\r\n";
  request += "\r\n";
  request += payload;

  client.println(request);

  unsigned long startRequest = millis();
  while(client.available() == 0) {
    yield();
    if(millis() - startRequest > 5000) {
      Serial.println("Could not connect to remote host (timeout)");
      client.stop();
      break;
    }
  }

  client.stop();
}

void fetch(String endpoint, String payload) {
  if(WEB_SECURE) {
    return fetchHttps(endpoint, payload);
  } else {
    return fetchHttp(endpoint, payload);
  }
}
