#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "config.h"
#include "AnimationData.h"
#include "ExceptionVisualizer.h"
#include "WiFiConnection.h"
#include "HttpConnection.h"
#include "APIHandler.h"
#include "WSConnection.h"
#include "FrameHandling.h"

void setup() {
  Serial.begin(115200);
  setupExceptionVisualizer();
  setupWiFiConnection();
  setupWSConnection();
  setupFrameHandling();
}

void loop() {
  maintainWSConnection();
  checkError();
}
