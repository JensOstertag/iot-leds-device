unsigned long ev_LastPing = 0;
bool ev_StartupAnimation = false;
bool ev_ErrorAnimation = false;

void fakeColor(JsonArray array, int r, int g, int b) {
  JsonObject color = array.createNestedObject();
  JsonObject rgb = color.createNestedObject("rgb");
  rgb["r"] = r;
  rgb["g"] = g;
  rgb["b"] = b;
  color["hex"] = "#000000"; // Not used by the code;
}

void setStartupAnimation() {
  if(ev_StartupAnimation) {
    return;
  }

  // Blue pulse
  JsonDocument document;
  JsonObject data = document.to<JsonObject>();
  JsonObject animation = data.createNestedObject("animation");
  animation["id"] = 0;
  animation["name"] = "STARTUP";
  animation["type"] = "PULSE";
  JsonArray colors = animation.createNestedArray("colors");
  fakeColor(colors, 0, 0, 255);
  fakeColor(colors, 0, 0, 31);
  animation["durationPerColor"] = 3;
  data["power"] = true;

  parseAnimation(data);

  ev_StartupAnimation = true;
}

void setErrorAnimation() {
  if(ev_ErrorAnimation) {
    return;
  }

  // TODO: Red pulse

  ev_ErrorAnimation = true;
}

void setupExceptionVisualizer() {
  ev_LastPing = millis();

  setStartupAnimation();
}

void refreshTimeout() {
  ev_LastPing = millis();
}

void checkError() {
  unsigned long currentTime = millis();
  if(ev_LastPing > currentTime) {
    // Overflow
    if(currentTime >= TIMEOUT_DURATION) {
      setErrorAnimation();
      return;
    }
  } else if(currentTime - ev_LastPing >= TIMEOUT_DURATION) {
    setErrorAnimation();
    return;
  }

  ev_ErrorAnimation = false;
}