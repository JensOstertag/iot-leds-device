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
  int startFreeHeap = ESP.getFreeHeap();
  frameHandling();
  int tempFreeHeap = ESP.getFreeHeap();
  if(tempFreeHeap != startFreeHeap) {
    Serial.println("Free Heap changed after frameHandling() call");
    Serial.print("Start: ");
    Serial.print(startFreeHeap);
    Serial.print(" --- Now: ");
    Serial.println(tempFreeHeap);
  }
  maintainWSConnection();
  tempFreeHeap = ESP.getFreeHeap();
  if(tempFreeHeap != startFreeHeap) {
    Serial.println("Free Heap changed after maintainWSConnection() call");
    Serial.print("Start: ");
    Serial.print(startFreeHeap);
    Serial.print(" --- Now: ");
    Serial.println(tempFreeHeap);
  }
  checkError();
  tempFreeHeap = ESP.getFreeHeap();
  if(tempFreeHeap != startFreeHeap) {
    Serial.println("Free Heap changed after checkError() call");
    Serial.print("Start: ");
    Serial.print(startFreeHeap);
    Serial.print(" --- Now: ");
    Serial.println(tempFreeHeap);
  }
  render();
  tempFreeHeap = ESP.getFreeHeap();
  if(tempFreeHeap != startFreeHeap) {
    Serial.println("Free Heap changed after render() call");
    Serial.print("Start: ");
    Serial.print(startFreeHeap);
    Serial.print(" --- Now: ");
    Serial.println(tempFreeHeap);
  }

  if(millis() % 1000 == 0) {
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());
    delay(1);
  }
}
