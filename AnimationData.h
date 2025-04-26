#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel ledStrip(LED_AMOUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int ad_Id;
String ad_Type;
int ad_DurationPerColor;
int ad_ColorAmount;
int ad_Colors[MAX_COLORS_PER_ANIMATION][3];

float ad_CurrentColor;

void setupRenderer() {
  ledStrip.begin();
  ledStrip.show();
  ledStrip.setBrightness(LED_BRIGHTNESS);
  ledStrip.clear();

  ad_CurrentColor = 0;
}

void resetAnimation() {
  ad_Id = 0;
  ad_Type = "";
  ad_DurationPerColor = 0;
  ad_ColorAmount = 0;
  for(int i = 0; i < MAX_COLORS_PER_ANIMATION; i++) {
    for(int j = 0; j < 3; j++) {
      ad_Colors[i][j] = 0;
    }
  }

  ad_CurrentColor = 0;
}

void parseAnimation(JsonObject payload) {
  resetAnimation();

  JsonObject animationObject = payload["animation"];

  ad_Id = animationObject["id"].as<int>();
  ad_Type = String(animationObject["type"].as<const char*>());
  ad_DurationPerColor = animationObject["durationPerColor"].as<float>();
  JsonArray colorObjects = animationObject["colors"];
  ad_ColorAmount = colorObjects.size();
  if(ad_ColorAmount > MAX_COLORS_PER_ANIMATION) {
    ad_ColorAmount = MAX_COLORS_PER_ANIMATION;
  }

  Serial.print("colors: ");
  Serial.println(colorObjects.size());

  int i = 0;
  for(JsonVariant colorObject : colorObjects) {
    if(i >= ad_ColorAmount) {
      break;
    }

    ad_Colors[i][0] = colorObject["rgb"]["r"].as<int>();
    ad_Colors[i][1] = colorObject["rgb"]["g"].as<int>();
    ad_Colors[i][2] = colorObject["rgb"]["b"].as<int>();

    Serial.print(ad_Colors[i][0]);
    Serial.print(" - ");
    Serial.print(ad_Colors[i][1]);
    Serial.print(" - ");
    Serial.println(ad_Colors[i][2]);

    i++;
  }
}

void render() {
  Serial.print("OLD: ");
  Serial.print(ad_CurrentColor);
  float oldColor = ad_CurrentColor;
  float progressDuringFrame = 0;
  Serial.print(" - DT: ");
  Serial.print(deltaTime());
  Serial.print(" - DUR: ");
  Serial.print(ad_DurationPerColor);
  if(ad_DurationPerColor != 0) {
    progressDuringFrame = deltaTime() / float(ad_DurationPerColor);
  }
  Serial.print(" - PROG: ");
  Serial.print(progressDuringFrame);
  float newColor = oldColor + progressDuringFrame;
  Serial.print(" - NEW: ");
  Serial.println(newColor);
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
