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
