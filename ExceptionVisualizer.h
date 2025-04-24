unsigned long ev_LastPing = 0;
bool ev_StartupAnimation = false;
bool ev_ErrorAnimation = false;

void setStartupAnimation() {
  if(ev_StartupAnimation) {
    return;
  }

  // TODO: Blue pulse

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