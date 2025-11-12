#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

String fetchHttps(String endpoint, String payload) {
  JsonDocument emptyResponse;

  WiFiClientSecure client;
  client.setInsecure();

  Serial.println("Connecting to Web-API host");
  if(!client.connect(WEB_HOST, WEB_PORT)) {
    Serial.println("Could not connect to remote host");
    return "";
  }
  Serial.println("Connected");

  Serial.println("Sending HTTP request");
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
  Serial.println("HTTP request sent");

  Serial.println("Waiting for response data");
  unsigned long startRequest = millis();
  while(client.available() == 0) {
    yield();
    if(millis() - startRequest > 5000) {
      Serial.println("Could not connect to remote host (timeout)");
      client.stop();
      return "";
    }
  }
  Serial.println("Response received");

  Serial.println("Reading HTTP header");
  bool chunked = false;
  while(client.available() != 0) {
    String line = client.readStringUntil('\n');

    // Check if the response is chunked
    if((line.indexOf("Transfer-Encoding:") >= 0 || line.indexOf("transfer-encoding:") >= 0) && (line.indexOf("chunked") >= 0 || line.indexOf("Chunked") >= 0)) {
      chunked = true;
    }

    // End of header
    if(line.equals("\r") || line.length() == 0) {
      break;
    }
  }
  Serial.println("HTTP header read");

  Serial.println("Reading HTTP body");
  if(chunked) {
    Serial.println("In chunked mode");
  }
  String json = "";
  while(client.available() != 0) {
    String line = client.readStringUntil('\n');
    if(!chunked) {
      json = line;
      break;
    }

    if(line.charAt(line.length() - 1) == '\r') {
      line.setCharAt(line.length() - 1, '\0');
    }
    long chunkSize = strtol(line.c_str(), 0, 16);

    // Repeat until the entire response was read
    while(chunkSize > 0) {
      // Wait until the next character is available
      unsigned long startWait = millis();
      while(client.available() == 0) {
        yield();
        if(millis() - startWait > 500) {
          Serial.println("Could not read next chunk (timeout)");
          client.stop();
          return "";
        }
      }

      // Append the character to the response
      char c = client.read();
      json += c;
      chunkSize--;
    }

    // Stop after the first response line
    break;
  }
  Serial.println("HTTP body read");

  client.stop();
  return json;
}

String fetchHttp(String endpoint, String payload) {
  WiFiClient client;

  Serial.println("Connecting to Web-API host");
  if(!client.connect(WEB_HOST, WEB_PORT)) {
    Serial.println("Could not connect to remote host");
    return "";
  }
  Serial.println("Connected");

  Serial.println("Sending HTTP request");
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
  Serial.println("HTTP request sent");

  Serial.println("Waiting for response data");
  unsigned long startRequest = millis();
  while(client.available() == 0) {
    yield();
    if(millis() - startRequest > 5000) {
      Serial.println("Could not connect to remote host (timeout)");
      client.stop();
      return "";
    }
  }
  Serial.println("Response received");

  Serial.println("Reading HTTP header");
  bool chunked = false;
  while(client.available() != 0) {
    String line = client.readStringUntil('\n');

    // Check if the response is chunked
    if((line.indexOf("Transfer-Encoding:") >= 0 || line.indexOf("transfer-encoding:") >= 0) && (line.indexOf("chunked") >= 0 || line.indexOf("Chunked") >= 0)) {
      chunked = true;
    }

    // End of header
    if(line.equals("\r") || line.length() == 0) {
      break;
    }
  }
  Serial.println("HTTP header read");

  Serial.println("Reading HTTP body");
  if(chunked) {
    Serial.println("In chunked mode");
  }
  String json = "";
  while(client.available() != 0) {
    String line = client.readStringUntil('\n');
    if(!chunked) {
      json = line;
      break;
    }

    if(line.charAt(line.length() - 1) == '\r') {
      line.setCharAt(line.length() - 1, '\0');
    }
    long chunkSize = strtol(line.c_str(), 0, 16);

    // Repeat until the entire response was read
    while(chunkSize > 0) {
      // Wait until the next character is available
      unsigned long startWait = millis();
      while(client.available() == 0) {
        yield();
        if(millis() - startWait > 500) {
          Serial.println("Could not read next chunk (timeout)");
          client.stop();
          return "";
        }
      }

      // Append the character to the response
      char c = client.read();
      json += c;
      chunkSize--;
    }

    // Stop after the first response line
    break;
  }
  Serial.println("HTTP body read");

  client.stop();
  return json;
}

String fetch(String endpoint, String payload) {
  if(WEB_SECURE) {
    Serial.println("Fetching HTTPS API");
    return fetchHttps(endpoint, payload);
  } else {
    Serial.println("Fetching HTTP API");
    return fetchHttp(endpoint, payload);
  }
}
