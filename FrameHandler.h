long fh_LastTime;
float fh_DeltaTime;

void setupFrameHandler() {
  fh_LastTime = millis();
}

float deltaTime() {
  return fh_DeltaTime;
}

void frameHandler() {
  long durationOfLastLoop = millis() - fh_LastTime;
  if(durationOfLastLoop < 0) {
    durationOfLastLoop = 0;
  }
  fh_DeltaTime = ((float) durationOfLastLoop) / ((float) 1000);
  fh_LastTime = millis();
}
