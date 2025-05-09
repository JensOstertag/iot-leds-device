#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "config.h"
#include "CalculationUtil.h"
#include "Encryption.h"
#include "FrameHandling.h"
#include "AnimationData.h"
#include "ExceptionVisualizer.h"
#include "WiFiConnection.h"
#include "WSConnection.h"

void setup() {
  Serial.begin(115200);
  setupFrameHandling();
  setupRenderer();
  setupExceptionVisualizer();
  setupWiFiConnection();
  setupWSConnection();
}

void loop() {
  frameHandling();
  maintainWSConnection();
  checkError();
  render();
}
