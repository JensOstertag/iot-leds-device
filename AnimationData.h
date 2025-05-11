#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel ledStrip(LED_AMOUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int ad_Id = -1;
String ad_Type;
int ad_DurationPerColor;
int ad_ColorAmount;
int ad_Colors[MAX_COLORS_PER_ANIMATION][3];
bool ad_Power;

float ad_CurrentColor = 0;

void setupRenderer() {
  ledStrip.begin();
  ledStrip.show();
  ledStrip.setBrightness(LED_BRIGHTNESS);
  ledStrip.clear();
}

void resetAnimation() {
  ad_Id = 0;
  ad_Type = "";
  ad_DurationPerColor = 0;
  ad_ColorAmount = 0;
  ad_Power = false;
  for(int i = 0; i < MAX_COLORS_PER_ANIMATION; i++) {
    for(int j = 0; j < 3; j++) {
      ad_Colors[i][j] = 0;
    }
  }

  ad_CurrentColor = 0;
}

void parseAnimation(JsonObject payload) {
  JsonObject animationObject = payload["animation"];

  int newAnimationId = animationObject["id"].as<int>();
  if(newAnimationId == ad_Id) {
    return;
  }

  resetAnimation();

  ad_Id = newAnimationId;
  ad_Type = String(animationObject["type"].as<const char*>());
  ad_DurationPerColor = animationObject["durationPerColor"].as<float>();
  JsonArray colorObjects = animationObject["colors"];
  ad_ColorAmount = colorObjects.size();
  if(ad_ColorAmount > MAX_COLORS_PER_ANIMATION) {
    ad_ColorAmount = MAX_COLORS_PER_ANIMATION;
  }
  ad_Power = payload["power"].as<bool>();

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

void render() {
  if(ad_Power == false) {
    ledStrip.clear();
    return;
  }

  float oldColor = ad_CurrentColor;
  float progressDuringFrame = 0;
  if(ad_DurationPerColor != 0) {
    progressDuringFrame = deltaTime() / float(ad_DurationPerColor);
  }
  float newColor = oldColor + progressDuringFrame;
  while(newColor >= ad_ColorAmount) {
    newColor -= ad_ColorAmount;
  }
  ad_CurrentColor = newColor;
  ledStrip.clear();

  if(ad_ColorAmount == 0) {
    return;
  }

  if(ad_Type.equals("STATIC")) {
    int fromColor = int(ad_CurrentColor) % ad_ColorAmount;

    int targetR = ad_Colors[fromColor][0];
    int targetG = ad_Colors[fromColor][1];
    int targetB = ad_Colors[fromColor][2];

    for(int i = 0; i < ledStrip.numPixels(); i++) {
      ledStrip.setPixelColor(i, ledStrip.Color(
        targetR,
        targetG,
        targetB
      ));
    }

    ledStrip.show();
  } else if(ad_Type.equals("SLIDE")) {
    float progressPerLed = float(ad_ColorAmount) / float(ledStrip.numPixels());

    float ledProgress = ad_CurrentColor;
    for(int i = 0; i < ledStrip.numPixels(); i++) {
      if(i > 0) {
        ledProgress += progressPerLed;
        while(ledProgress >= ad_ColorAmount) {
          ledProgress -= ad_ColorAmount;
        }
      }

      int fromColor = int(ledProgress) % ad_ColorAmount;
      int toColor = (fromColor + 1) % ad_ColorAmount;
      float colorProgress = ledProgress - fromColor;

      int fromR = ad_Colors[fromColor][0];
      int fromG = ad_Colors[fromColor][1];
      int fromB = ad_Colors[fromColor][2];

      int toR = ad_Colors[toColor][0];
      int toG = ad_Colors[toColor][1];
      int toB = ad_Colors[toColor][2];

      int targetR = ratio(fromR, toR, colorProgress);
      int targetG = ratio(fromG, toG, colorProgress);
      int targetB = ratio(fromB, toB, colorProgress);

      ledStrip.setPixelColor(i, ledStrip.Color(
        targetR,
        targetG,
        targetB
      ));
    }

    ledStrip.show();
  } else if(ad_Type.equals("PULSE")) {
    int fromColor = int(ad_CurrentColor) % ad_ColorAmount;
    int toColor = (fromColor + 1) % ad_ColorAmount;
    float colorProgress = ad_CurrentColor - fromColor;

    int fromR = ad_Colors[fromColor][0];
    int fromG = ad_Colors[fromColor][1];
    int fromB = ad_Colors[fromColor][2];

    int toR = ad_Colors[toColor][0];
    int toG = ad_Colors[toColor][1];
    int toB = ad_Colors[toColor][2];

    int targetR = ratio(fromR, toR, colorProgress);
    int targetG = ratio(fromG, toG, colorProgress);
    int targetB = ratio(fromB, toB, colorProgress);

    for(int i = 0; i < ledStrip.numPixels(); i++) {
      ledStrip.setPixelColor(i, ledStrip.Color(
        targetR,
        targetG,
        targetB
      ));
    }

    ledStrip.show();
  }
}
