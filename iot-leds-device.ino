#include "config.h"
#include "framehandling.h"
#include "apiconnection.h"
#include "animations.h"

void setup() {
  Serial.begin(115200);
  setupWiFiConnection();
  setupFrameHandling();

  fetch();
}

void loop() {
  // frameHandling();

  delay(1);
}
