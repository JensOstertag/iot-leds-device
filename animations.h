class Animation {
  public:
    int id;
    String animationType;
    int durationPerColor;
    int colorAmount;
    int colors[MAX_COLORS_PER_ANIMATION][3];

    float animationProgress = 0;
};

Animation parseAnimation(JsonObject animationObject) {
  Animation animation;
  animation.id = animationObject["id"].as<int>();
  animation.animationType = String(animationObject["type"].as<const char*>());
  animation.durationPerColor = animationObject["durationPerColor"].as<float>();
  JsonArray colorObjects = animationObject["colors"];
  animation.colorAmount = colorObjects.size();
  if(animation.colorAmount > MAX_COLORS_PER_ANIMATION) {
    animation.colorAmount = MAX_COLORS_PER_ANIMATION;
  }

  int i = 0;
  for(JsonVariant colorObject : colorObjects) {
    if(i >= animation.colorAmount) {
      break;
    }

    animation.colors[i][0] = colorObject["rgb"]["r"].as<int>();
    animation.colors[i][1] = colorObject["rgb"]["g"].as<int>();
    animation.colors[i][2] = colorObject["rgb"]["b"].as<int>();

    i++;
  }

  return animation;
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
  Animation animation = parseAnimation(response["data"]["animation"]);

  Serial.println("CP5");
}
