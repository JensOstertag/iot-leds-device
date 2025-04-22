int ad_Id;
String ad_Type;
int ad_DurationPerColor;
int ad_ColorAmount;
int ad_Colors[MAX_COLORS_PER_ANIMATION][3];

void parseAnimation(JsonObject animationObject) {
  ad_Id = animationObject["id"].as<int>();
  ad_Type = String(animationObject["type"].as<const char*>());
  ad_DurationPerColor = animationObject["durationPerColor"].as<float>();
  JsonArray colorObjects = animationObject["colors"];
  ad_ColorAmount = colorObjects.size();
  if(ad_ColorAmount > MAX_COLORS_PER_ANIMATION) {
    ad_ColorAmount = MAX_COLORS_PER_ANIMATION;
  }

  int i = 0;
  for(JsonVariant colorObject : colorObjects) {
    if(i >= ad_ColorAmount) {
      break;
    }

    ad_Colors[i][0] = colorObject["rgb"]["r"].as<int>();
    ad_Colors[i][1] = colorObject["rgb"]["g"].as<int>();
    ad_Colors[i][2] = colorObject["rgb"]["b"].as<int>();

    i++;
  }
}

void fetchAnimation() {
  // Build POST payload
  JsonDocument postData;
  postData["deviceUid"] = DEV_UID;
  postData["deviceApiKey"] = DEV_API_KEY;

  String requestPayload;
  serializeJson(postData, requestPayload);

  Serial.println("CP1");

  // Fetch API
  String endpoint = String(WEB_HOST) + "/api/get-device-animation";
  JsonDocument response = String(WEB_HOST).startsWith("https") ? fetchHttps(endpoint, requestPayload) : fetchHttp(endpoint, requestPayload);

  Serial.println("CP2");

  if(!response["code"]) {
    return;
  }

  Serial.println("CP3");

  if(response["code"] != 200) {
    Serial.print("An error occurred whilst attempting to fetch the API. Received error code ");
    Serial.print(response["code"].as<int>());
    Serial.print(" (");
    Serial.print(response["message"].as<const char*>());
    Serial.println(").");
    return;
  }

  Serial.println("CP4");

  // Parse animation
  parseAnimation(response["data"]["animation"]);

  Serial.println("CP5");
}
