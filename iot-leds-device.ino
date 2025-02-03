#include "config.h"
#include "framehandling.h"
#include "apiconnection.h"
#include "animations.h"

void setup() {
  Serial.begin(115200);
  setupWiFiConnection();
  setupFrameHandling();

  long start = millis();
  fetch();
  long end = millis();
  long duration = end - start;
  Serial.print("Done, took ");
  Serial.print(duration);
  Serial.println(" ms");
}

void loop() {
  // frameHandling();

  delay(1);
}
