#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "config.h"
#include "APIConnection.h"
#include "AnimationData.h"
#include "FrameHandling.h"

void setup() {
  Serial.begin(115200);
  setupWiFiConnection();
  setupFrameHandling();

  long start = millis();
  fetchAnimation();
  long end = millis();
  long duration = end - start;
  Serial.print("Done, took ");
  Serial.print(duration);
  Serial.println(" ms");
}

void loop() {
  // frameHandling();

  // delay(1)
}
