#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "config.h"
#include "CalculationUtil.h"
#include "FrameHandler.h"
#include "AnimationData.h"
#include "ExceptionVisualizer.h"
#include "OTAHandler.h"
#include "Animator.h"
#include "WiFiConnection.h"
#include "HTTPConnection.h"
#include "APIHandler.h"

void setup() {
  Serial.begin(115200);
  setupFrameHandler();
  setupRenderer();
  setupExceptionVisualizer();
  setupWiFiConnection();
  setupApiHandler();
  setupOtaHandler();
}

unsigned long lastAnimationFetch = 0;
void loop() {
  if(lastAnimationFetch == 0 || lastAnimationFetch > millis() || millis() - lastAnimationFetch >= 60000) {
    fetchAnimation();
    lastAnimationFetch = millis();
  }

  animationLoop();
}
