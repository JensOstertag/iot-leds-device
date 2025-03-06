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
