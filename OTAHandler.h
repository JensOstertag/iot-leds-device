#include <ArduinoOTA.h>

void setupOtaHandler() {
  String hostname = "iot-leds-";
  hostname += DEV_UID;
  char* hostnameCharArray;
  hostname.toCharArray(hostnameCharArray, hostname.length());
  ArduinoOTA.setHostname(hostnameCharArray);
  ArduinoOTA.onStart([]() {
    resetAnimation();
    render();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    (void) error;
    ESP.restart();
  });

  ArduinoOTA.begin();
}

void otaHandler() {
  ArduinoOTA.handle();
}
