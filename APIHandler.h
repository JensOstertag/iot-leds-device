JsonDocument api_AuthenticationObject;
String api_AuthenticationRaw;

void setupApiHandler() {
  api_AuthenticationObject["deviceUid"] = DEV_UID;
  api_AuthenticationObject["deviceApiKey"] = DEV_API_KEY;

  serializeJson(api_AuthenticationObject, api_AuthenticationRaw);
}

void fetchAnimation() {
  int retries = 0;
  bool success = false;
  while(!success && retries < 3) {
    if(retries != 0) {
      Serial.println("Retrying...");
    }
    retries++;
    Serial.println("Fetching Web-API");
    String responseRaw = fetch("/api/get-device-animation", api_AuthenticationRaw);
    Serial.println("Received response from Web-API");

    if(responseRaw.length() <= 0) {
      Serial.println("Received empty response from the server or error");
      continue;
    }

    JsonDocument response;
    DeserializationError deserializationError = deserializeJson(response, responseRaw);

    if(deserializationError) {
      Serial.println("Received non-parsable response from the server:");
      Serial.println(responseRaw);
      continue;
    }

    if(response["code"].as<int>() != 200) {
      Serial.println("Received bad response from the server:");
      Serial.println(responseRaw);
      continue;
    }

    Serial.println("Parsing response from Web-API");
    parseAnimation(response["data"]);

    success = true;
  }

  if(!success) {
    setErrorAnimation();
  }
}
